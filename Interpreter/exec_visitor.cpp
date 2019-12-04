#include "pch.h"
#include "memory_table.h"
#include "exec_visitor.h"
#include "builtin_type_symbol.h"
#include "type_impl.h"
#include "procedure_symbol.h"

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

	this->register_visit_result({
		builtin_type_symbol::get_for_builtin_type(ast::builtin_type::integer),
		0
	});
}

void exec_visitor::visit(ast::assign& assign)
{
	auto& ctx = this->m_stack.current_context();

	// Get result
	const auto assign_expr = assign.right();
	auto result = this->visit_with_result(*assign_expr);

	// Register result
	const auto identifier = assign.left()->identifier();
	const auto var_symbol = ctx.symbols.get<variable_symbol>(identifier);
	const auto type = ctx.symbols.get<type_symbol>(var_symbol->variable().type()->identifier());
	const auto& assign_type_impl = type->type_impl();

	if (!assign_type_impl.supports_implicit_type_conversion_from(result.type, token_type::assign))
	{
		throw runtime_type_error(L"Attempting to assign variable " + var_symbol->to_string() + L" (which is of type " + type->to_string() + L") from expression with invalid type: " + result.type->to_string(), assign.get_line_info());
	}

	type_operation_context type_operation_context = {
		this->get_interpreter_context(),
		this->get_current_scope().symbols,
		assign.get_line_info()
	};
	assign_type_impl.implicit_type_conversion(result, type_operation_context);
	
	ctx.memory->set(var_symbol, result.value);
}

void exec_visitor::visit(ast::var& var)
{
	auto& ctx = this->m_stack.current_context();

	// Lookup in current scope
	const auto var_symbol = ctx.symbols.get<variable_symbol>(var.identifier());
	
	const auto var_val = ctx.memory->get(var_symbol);
	const auto builtin_type = ctx.symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());
	
	this->register_visit_result({builtin_type, var_val});
}

void exec_visitor::visit(ast::block& block)
{
	// Blocks don't have their own scope. Procedures and programs do.
	ast_node_visitor::visit(block);
}

void exec_visitor::visit(ast::program& program)
{
	// Create an initial scope. This can be queried later.
	this->m_stack.create_global_scope(program);
	
	ast_node_visitor::visit(program);
}

void exec_visitor::visit(ast::procedure&)
{
	// We do nothing. Procedures are called so we don't interpret them until the moment
	// they are called with their own parameters.
}

void exec_visitor::visit(ast::procedure_call& procedure_call)
{
	auto& ctx = this->m_stack.current_context();

	// General context for type operations
	type_operation_context type_operation_context = {
		this->get_interpreter_context(),
		this->get_current_scope().symbols,
		procedure_call.get_line_info()
	};

	// Find the referenced procedure
	const auto procedure_symbol = ctx.symbols.get<::procedure_symbol>(procedure_call.procedure_identifier());
	const auto& procedure_params = procedure_symbol->procedure().params();
	const auto& procedure_args = procedure_call.args();

	// Verify that even the correct number of arguments have been given. Let's do this while iterating the args and params.
	// Note on parameters vs arguments: Parameters are defined, arguments are given.
	
	// We now have to process each of the parameters before the invocation. We get their value,
	// then put their values in the memory table for the procedure. After this the invocation is fairly simple.
	auto& proc_ctx = this->m_stack.new_scope(procedure_symbol->procedure().symbol_table());
	
	auto param_iterator = procedure_params.begin();
	const auto params_end = procedure_params.end();
	auto arg_iterator = procedure_args.begin();
	const auto args_end = procedure_args.end();

	while (param_iterator != procedure_params.end() || arg_iterator != args_end)
	{
		// We only get in this loop if there are parameters or arguments left to iterate.
		// This means that we can have parameters that have no corresponding argument,
		// or arguments for which no parameter has been declared.
		if (param_iterator == procedure_params.end())
		{
			throw exec_error(L"In a call to " + procedure_symbol->to_string() + L" too many arguments have been provided", (*arg_iterator)->get_line_info());
		}

		if (arg_iterator == args_end)
		{
			throw exec_error(L"In a call to " + procedure_symbol->to_string() + L" no argument has been provided for this parameter: " + (*param_iterator)->identifier(), procedure_call.get_line_info());
		}

		const ast::ast_ptr arg = *arg_iterator;
		const ast::ast_node_ptr<ast::var_decl> param = *param_iterator;

		// Set line info
		type_operation_context.line_info = arg->get_line_info();

		// Resolve the value
		eval_value arg_value = this->visit_with_result(*arg);

		// Assign it with conversion
		auto param_type = proc_ctx.symbols.get<type_symbol>(param->type()->identifier());
		
		const auto& assign_type_impl = param_type->type_impl();
		if (!assign_type_impl.supports_implicit_type_conversion_from(arg_value.type, token_type::assign))
		{
			throw runtime_type_error(L"Attempting to assign parameter " + param->identifier() + L" of " + procedure_symbol->to_string() + L" with invalid type: " + arg_value.type->to_string(), arg->get_line_info());
		}

		assign_type_impl.implicit_type_conversion(arg_value, type_operation_context);

		auto param_var = proc_ctx.symbols.get<variable_symbol>(param->identifier());
		proc_ctx.memory->set(param_var, arg_value.value);

		++param_iterator;
		++arg_iterator;
	}
	// We have now bound all arguments in for the procedure. We can now evaluate the body.
	procedure_symbol->procedure().block()->accept(*this);

	// Go back to previous context
	this->m_stack.pop();
}

std::shared_ptr<scope_context> exec_visitor::global_scope() const
{
	return this->m_stack.global_scope();
}
