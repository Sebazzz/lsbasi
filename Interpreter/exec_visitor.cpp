#include "exec_visitor.h"
#include "interpret_except.h"

void variable_register::set_from_parent(const ast::var_identifier& identifier, double value)
{
	var v { value, true };
	this->m_variables.insert_or_assign(identifier, v);
}

double variable_register::get(const ast::var_identifier& identifier)
{
	const auto var_val = this->m_variables.find(identifier);

	// Check if it exists
	if (var_val == this->m_variables.end())
	{
		throw interpret_except("Unknown variable in this scope", identifier);
	}

	return var_val->second.value;
}

void variable_register::set(const ast::var_identifier& identifier, double value)
{
	var v { value, false };
	this->m_variables.insert_or_assign(identifier, v);
}

std::unique_ptr<variable_register> variable_register::create_from_parent_scope(const variable_register* parent)
{
	std::unique_ptr<variable_register> var_reg(new variable_register());

	for (auto && pair : parent->m_variables)
	{
		var_reg->set_from_parent(pair.first, pair.second.value);
	}
	
	return var_reg;
}

void variable_register::copy_to_parent()
{
	if (this->m_previous == nullptr)
	{
		return;
	}
	
	for (auto && pair : this->m_variables)
	{
		auto&& parent_var = this->m_previous->m_variables.find(pair.first);
		parent_var->second.value = pair.second.value;
	}
}

scope_context& scope_manager::current_context()
{
	if (this->m_scope.empty())
	{
		throw interpret_except("Expected non-empty stack");
	}

	return this->m_scope.top();
}

scope_context& scope_manager::push()
{
	if (!this->m_scope.empty())
	{
		const auto& top_scope = this->m_scope.top();
		this->m_scope.push(scope_context { variable_register::create_from_parent_scope(top_scope.variables.get()) });
	} else
	{
		this->m_scope.push(scope_context { std::make_unique<variable_register>() });
	}
	
	return this->m_scope.top();
}

void scope_manager::pop()
{
	if (this->m_scope.empty())
	{
		throw interpret_except("Expected non-empty stack");
	}

	this->m_scope.top().variables->copy_to_parent();
	this->m_scope.pop();
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
	ctx.variables->set(identifier, result);

	this->m_last_value = identifier + L" <- " + std::to_wstring(result);
}

void exec_visitor::visit(ast::var& var)
{
	auto& ctx = this->m_stack.current_context();

	// Lookup in current scope
	const auto var_val = ctx.variables->get(var.identifier());
	this->register_visit_result(var_val);
}

std::wstring exec_visitor::last_value() const
{
	return this->m_last_value;
}
