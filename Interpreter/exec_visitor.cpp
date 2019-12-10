#include "pch.h"
#include "memory_table.h"
#include "exec_visitor.h"
#include "builtin_type_symbol.h"
#include "type_impl.h"
#include "routine_symbol.h"

scope_context& exec_visitor::get_current_scope()
{
	return this->m_stack.current_context();
}

interpreter_context& exec_visitor::get_interpreter_context() const
{
	return this->m_interpreter_context;
}

exec_visitor::exec_visitor(interpreter_context& interpreter_context): m_interpreter_context(interpreter_context)
{
}

void exec_visitor::visit(ast::ast_node& node)
{
	eval_visitor::visit(node);
}

void exec_visitor::visit(ast::compound& compound)
{
	ast_node_visitor::visit(compound);

	this->register_void_result();
}

void exec_visitor::visit(ast::assign& assign)
{
	auto& ctx = this->m_stack.current_context();

	// Get result
	const auto assign_expr = assign.right();
	auto result = this->visit_with_result(*assign_expr);

	// Register result
	const auto assign_target_symbol = assign.left()->assignment_symbol();
	const auto type = assign.left()->type_symbol();
	const auto& assign_type_impl = type->type_impl();

	if (!assign_type_impl.supports_implicit_type_conversion_from(result.type, token_type::assign))
	{
		throw runtime_type_error(L"Attempting to assign variable " + assign_target_symbol->to_string() + L" (which is of type " + type->to_string() + L") from expression with invalid type: " + result.type->to_string(), assign.get_line_info());
	}

	type_operation_context type_operation_context = {
		this->get_interpreter_context(),
		this->get_current_scope().symbols,
		assign.get_line_info()
	};
	assign_type_impl.implicit_type_conversion(result, type_operation_context);
	
	ctx.memory->set(assign_target_symbol, result.value);
}

void exec_visitor::visit(ast::assignment_target& assignment_target)
{
	auto& ctx = this->m_stack.current_context();

	// Lookup in current scope
	const auto var_symbol = assignment_target.assignment_symbol();
	const auto var_val = ctx.memory->get(var_symbol);
	const auto var_type = assignment_target.type_symbol();
	
	this->register_visit_result({var_type, var_val});
}

void exec_visitor::visit(ast::block& block)
{
	ast_node_visitor::visit(block);
}

void exec_visitor::visit(ast::program& program)
{
	// Create an initial scope. This can be queried later.
	this->m_stack.create_global_scope(program);
	
	ast_node_visitor::visit(program);
}

void exec_visitor::visit(ast::routine&)
{
	// We do nothing. Procedures are called so we don't interpret them until the moment
	// they are called with their own parameters.
}

void exec_visitor::visit(ast::routine_call& procedure_call)
{
	// General context for type operations
	type_operation_context type_operation_context = {
		this->get_interpreter_context(),
		this->get_current_scope().symbols,
		procedure_call.get_line_info()
	};

	// Find the referenced procedure
	const auto routine_symbol = procedure_call.routine_symbol();
	const auto& routine_params = routine_symbol->params();
	const auto& procedure_args = procedure_call.args();

	// Verify that even the correct number of arguments have been given. Let's do this while iterating the args and params.
	// Note on parameters vs arguments: Parameters are defined, arguments are given.
	
	// We now have to process each of the parameters before the invocation. We get their value,
	// then put their values in the memory table for the procedure. After this the invocation is fairly simple.
	auto proc_ctx = this->m_stack.prepare_new_scope(routine_symbol->symbol_table());
	
	auto param_iterator = routine_params.begin();
	const auto params_end = routine_params.end();
	auto arg_iterator = procedure_args.begin();
	const auto args_end = procedure_args.end();

	while (param_iterator != routine_params.end() || arg_iterator != args_end)
	{
		// We only get in this loop if there are parameters or arguments left to iterate.
		// This means that we can have parameters that have no corresponding argument,
		// or arguments for which no parameter has been declared.
		if (param_iterator == routine_params.end())
		{
			throw exec_error(L"In a call to " + routine_symbol->to_string() + L" too many arguments have been provided", (*arg_iterator)->get_line_info());
		}

		if (arg_iterator == args_end)
		{
			throw exec_error(L"In a call to " + routine_symbol->to_string() + L" no argument has been provided for this parameter: " + (*param_iterator)->identifier(), procedure_call.get_line_info());
		}

		const ast::ast_ptr arg = *arg_iterator;
		const ast::ast_node_ptr<ast::var_decl> param = *param_iterator;

		// Set line info
		type_operation_context.line_info = arg->get_line_info();

		// Resolve the value
		eval_value arg_value = this->visit_with_result(*arg);

		// Assign it with conversion
		auto param_type = param->type()->type_symbol();
		
		const auto& assign_type_impl = param_type->type_impl();
		if (!assign_type_impl.supports_implicit_type_conversion_from(arg_value.type, token_type::assign))
		{
			throw runtime_type_error(L"Attempting to assign parameter " + param->identifier() + L" of " + routine_symbol->to_string() + L" with invalid type: " + arg_value.type->to_string(), arg->get_line_info());
		}

		assign_type_impl.implicit_type_conversion(arg_value, type_operation_context);

		auto param_var = param->variable_symbol();
		proc_ctx.memory->set(param_var, arg_value.value);

		++param_iterator;
		++arg_iterator;
	}
	
	// We have now bound all arguments in for the procedure. We can now execute the procedure

	// ... Enter call frame
	auto& current_stack = this->m_stack.apply_prepared_scope(proc_ctx);
	
	// ... Try to handle as a user defined procedure
	const auto user_defined_procedure = std::dynamic_pointer_cast<user_defined_routine_symbol>(routine_symbol);
	const auto builtin_procedure = std::dynamic_pointer_cast<builtin_routine_symbol>(routine_symbol);

	if (user_defined_procedure)
	{
		// For user defined procedures we need to evaluate the body
		user_defined_procedure->procedure().block()->accept(*this);
	} else if (builtin_procedure)
	{
		// Built-in procedures just need the scope to extract the variables they need
		builtin_procedure->invoke(current_stack);
	}
	else
	{
		throw internal_interpret_except(std::string("Unsupported procedure: ") + typeid(*routine_symbol).name());
	}

	// Register return value if function
	if (routine_symbol->is_function())
	{
		const auto return_value = current_stack.memory->get(routine_symbol);

		this->register_visit_result({
			routine_symbol->return_type(),
			return_value
		});
	} else
	{
		this->register_void_result();
	}

	// Go back to previous context
	this->m_stack.pop();
}

void exec_visitor::visit(ast::if_then_else& if_then_else)
{
	// For an if/then/else, we first need to test the expression and convert the result to a boolean.
	// If true we take the true branch, else the false branch.

	const auto ctx = this->get_current_scope();
	const auto boolean_type = ctx.symbols.get(ast::builtin_type::boolean);
	const auto& boolean_type_impl = boolean_type->type_impl();
	
	auto test_result = this->visit_with_result(*if_then_else.test_expression());
	if (!boolean_type_impl.supports_implicit_type_conversion_from(test_result.type, token_type::assign))
	{
		throw runtime_type_error(L"Expression is not implicitly convertible to a boolean", if_then_else.get_line_info());
	}

	type_operation_context type_operation_context = {
		this->get_interpreter_context(),
		this->get_current_scope().symbols,
		if_then_else.get_line_info()
	};
	boolean_type_impl.implicit_type_conversion(test_result, type_operation_context);

	// Now determine the branch to take
	const ast::if_block& branch_to_take = test_result.value.boolean_val == true ? if_then_else.true_block() : if_then_else.false_block();

	// ... Handle either a block or single statement
	if (branch_to_take.ast_ptr) this->visit_with_void_result(*branch_to_take.ast_ptr);

	if (branch_to_take.block_ptr)
	{
		// TODO: check whether we need to create a new scope
		this->visit_with_void_result(*branch_to_take.block_ptr);
	}
}

std::shared_ptr<scope_context> exec_visitor::global_scope() const
{
	return this->m_stack.global_scope();
}
