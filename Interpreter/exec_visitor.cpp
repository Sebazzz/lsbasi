#include "exec_visitor.h"
#include "interpret_except.h"

stack_context& stack::current_context()
{
	if (this->m_stack.empty())
	{
		throw interpret_except("Expected non-empty stack");
	}

	return this->m_stack.top();
}

stack_context& stack::push()
{
	// TODO: copy variables from current context to this context
	this->m_stack.push(stack_context());
	
	return this->m_stack.top();
}

void stack::pop()
{
	if (this->m_stack.empty())
	{
		throw interpret_except("Expected non-empty stack");
	}
	
	this->m_stack.pop();
}

exec_visitor::exec_visitor() = default;

void exec_visitor::visit(ast::ast_node& node)
{
	eval_visitor::visit(node);
}

void exec_visitor::visit(ast::compound& compound)
{
	this->m_stack.push();
	ast_node_visitor::visit(compound);
	this->m_stack.pop();

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
	ctx.variables.insert_or_assign(identifier, result);

	this->m_last_value = identifier + L" <- " + std::to_wstring(result);
}

void exec_visitor::visit(ast::var& var)
{
	auto& ctx = this->m_stack.current_context();

	// Lookup in current scope
	const auto var_val = ctx.variables.find(var.identifier());

	// Check if it exists
	if (var_val == ctx.variables.end())
	{
		throw interpret_except("Unknown variable in this scope");
	}

	this->register_visit_result(var_val->second);
}

std::wstring exec_visitor::last_value() const
{
	return this->m_last_value;
}
