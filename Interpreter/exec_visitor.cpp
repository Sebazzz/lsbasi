#include "pch.h"
#include "memory_table.h"
#include "exec_visitor.h"

exec_visitor::exec_visitor() = default;

void exec_visitor::visit(ast::ast_node& node)
{
	eval_visitor::visit(node);
}

void exec_visitor::visit(ast::compound& compound)
{
	ast_node_visitor::visit(compound);

	this->register_visit_result({0, ast::var_type::integer });
}

void exec_visitor::visit(ast::assign& assign)
{
	auto& ctx = this->m_stack.current_context();

	// Get result
	const auto assign_expr = assign.right();
	auto result = this->accept(*assign_expr);

	// Register result
	const auto identifier = assign.left()->identifier();

	// Implicit conversion if destination is real
	if (ctx.memory->get(identifier).type == ast::var_type::real && result.type == ast::var_type::integer)
	{
		result.value.real_val = result.value.int_val;
		result.type = ast::var_type::real;
	}
	
	ctx.memory->ensure_type(identifier, result.type);
	ctx.memory->set(identifier, result.value);

	this->m_last_value = identifier + L" <- " + result.to_string();
}

void exec_visitor::visit(ast::var& var)
{
	auto& ctx = this->m_stack.current_context();

	// Lookup in current scope
	const auto var_val = ctx.memory->get(var.identifier());
	this->register_visit_result(var_val);
}

void exec_visitor::visit(ast::var_decl& var_decl)
{
	auto& current_context = this->m_stack.current_context();

	current_context.memory->declare(var_decl.identifier(), var_decl.type());
}

void exec_visitor::visit(ast::block& block)
{
	// Blocks don't have their own scope. Procedures and programs do.
	ast_node_visitor::visit(block);
}

void exec_visitor::visit(ast::program& program)
{
	// Create an initial scope. This can be queried later.
	this->m_stack.create_global_scope();
	
	ast_node_visitor::visit(program);
}

std::wstring exec_visitor::last_value() const
{
	return this->m_last_value;
}

const scope_context& exec_visitor::global_scope() const
{
	return this->m_stack.global_scope();
}
