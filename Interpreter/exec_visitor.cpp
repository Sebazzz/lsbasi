#include "pch.h"
#include "memory_table.h"
#include "exec_visitor.h"
#include "builtin_type_symbol.h"
#include "type_impl.h"

exec_visitor::exec_visitor() = default;

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
	auto result = this->accept(*assign_expr);

	// Register result
	const auto identifier = assign.left()->identifier();
	const auto var_symbol = ctx.symbols.get<variable_symbol>(identifier);
	const auto type = ctx.symbols.get<type_symbol>(var_symbol->variable().type()->identifier());

	try
	{
		type->type_impl().convert_value(result);
	} catch (interpret_except& e)
	{
		throw interpret_except(L"Attempting to assign variable " + var_symbol->to_string() + L" with invalid type", e.what());
	}
	
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

std::shared_ptr<scope_context> exec_visitor::global_scope() const
{
	return this->m_stack.global_scope();
}
