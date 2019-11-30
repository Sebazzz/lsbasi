#include "pch.h"
#include "memory_table.h"
#include "exec_visitor.h"

void exec_visitor::ensure_type(const variable_symbol& var_symbol, ast::builtin_type type, symbol_table& symbols)
{
	const auto& builtin_type = dynamic_cast<builtin_type_symbol&>(*symbols.get(var_symbol.variable().type()->identifier()));

	if (builtin_type.type() != type)
	{
		std::wstring var_type = L"unknown";
		switch (type)
		{
		case ast::builtin_type::integer:
			var_type = L"integer";
			break;
		case ast::builtin_type::real:
			var_type = L"real";
			break;
		}
		
		throw interpret_except(L"Attempting to assign expression of type " + var_type + L" to " + var_symbol.to_string());
	}
}

exec_visitor::exec_visitor() = default;

void exec_visitor::visit(ast::ast_node& node)
{
	eval_visitor::visit(node);
}

void exec_visitor::visit(ast::compound& compound)
{
	ast_node_visitor::visit(compound);

	this->register_visit_result({ast::builtin_type::integer, 0 });
}

void exec_visitor::visit(ast::assign& assign)
{
	auto& ctx = this->m_stack.current_context();

	// Get result
	const auto assign_expr = assign.right();
	auto result = this->accept(*assign_expr);

	// Register result
	const auto identifier = assign.left()->identifier();
	const auto symbol = ctx.symbols.get(identifier);
	const auto& var_symbol = dynamic_cast<const variable_symbol&>(*symbol);
	const auto& builtin_type = dynamic_cast<builtin_type_symbol&>(*ctx.symbols.get(var_symbol.variable().type()->identifier()));

	// Implicit conversion if destination is of type real
	if (builtin_type.type() == ast::builtin_type::real && result.type == ast::builtin_type::integer)
	{
		result.value.real_val = result.value.int_val;
		result.type = ast::builtin_type::real;
	}
	
	this->ensure_type(var_symbol, result.type, ctx.symbols);
	ctx.memory->set(symbol, result.value);
}

void exec_visitor::visit(ast::var& var)
{
	auto& ctx = this->m_stack.current_context();

	// Lookup in current scope
	const auto symbol = ctx.symbols.get(var.identifier());
	const auto& var_symbol = dynamic_cast<const variable_symbol&>(*symbol);
	
	const auto var_val = ctx.memory->get(symbol);
	const auto& builtin_type = dynamic_cast<builtin_type_symbol&>(*ctx.symbols.get(var_symbol.variable().type()->identifier()));
	
	this->register_visit_result({builtin_type.type(), var_val});
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

std::shared_ptr<scope_context> exec_visitor::global_scope() const
{
	return this->m_stack.global_scope();
}
