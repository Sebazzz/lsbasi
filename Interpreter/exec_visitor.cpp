#include "pch.h"
#include "symbol_table.h"
#include "exec_visitor.h"

exec_visitor::exec_visitor() = default;

void exec_visitor::visit(ast::ast_node& node)
{
	eval_visitor::visit(node);
}

void exec_visitor::visit(ast::compound& compound)
{
	ast_node_visitor::visit(compound);

	this->register_visit_result(0);
}

void exec_visitor::visit(ast::assign& assign)
{
	auto& ctx = this->m_stack.current_context();

	// Get result
	const auto assign_expr = assign.right();
	const auto result = this->accept(*assign_expr);

	// Register result
	const auto identifier = assign.left()->identifier();
	ctx.symbols->set(identifier, result);

	this->m_last_value = identifier + L" <- " + std::to_wstring(result);
}

void exec_visitor::visit(ast::var& var)
{
	auto& ctx = this->m_stack.current_context();

	// Lookup in current scope
	const auto var_val = ctx.symbols->get(var.identifier());
	this->register_visit_result(var_val);
}

void exec_visitor::visit(ast::var_decl& var_decl)
{
	auto& current_context = this->m_stack.current_context();

	current_context.symbols->declare(var_decl.identifier(), var_decl.type());
}

void exec_visitor::visit(ast::block& block)
{
	this->m_stack.push();
	ast_node_visitor::visit(block);
	this->m_stack.pop();
}

std::wstring exec_visitor::last_value() const
{
	return this->m_last_value;
}
