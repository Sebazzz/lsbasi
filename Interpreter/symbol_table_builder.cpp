#include "pch.h"
#include "symbol.h"
#include "symbol_table_builder.h"

symbol_table_builder::symbol_table_builder(const std::wstring& scope_name, ::symbol_table* parent_table)
{
	this->m_symbol_table = std::make_shared<::symbol_table>(scope_name, parent_table);
}

void symbol_table_builder::ensure_symbol_table() const
{
	if (!this->m_symbol_table)
	{
		throw interpret_except("Symbol table is not declared. This is a failure in the interpreter.");
	}
}

void symbol_table_builder::visit(ast::ast_node& node)
{
	ast_node_visitor::visit(node);
}

void symbol_table_builder::visit(ast::var& variable)
{
	// Throws on failure
	auto unused = this->m_symbol_table->get(variable.identifier());
}

void symbol_table_builder::visit(ast::var_decl& var_decl)
{
	this->ensure_symbol_table();

	const auto symbol = make_symbol_ptr<variable_symbol>(var_decl);
	this->m_symbol_table->declare(symbol);
}

void symbol_table_builder::visit(ast::program& program)
{
	if (this->m_symbol_table)
	{
		throw interpret_except("Attempting to visit program while already having a symbol table");
	}

	// This is the first visit we have
	this->m_symbol_table = std::make_shared<::symbol_table>(program.identifier(), nullptr);

	program.m_symbol_table = this->m_symbol_table;
	program.block()->accept(*this);
}

void symbol_table_builder::visit(ast::procedure& procedure)
{
	const auto symbol = make_symbol_ptr<procedure_symbol>(procedure);
	
	if (this->m_symbol_table)
	{
		// We found a (nested) procedure. New scope.
		symbol_table_builder nested_scope_visitor(procedure.identifier(), this->m_symbol_table.get());
		nested_scope_visitor.symbol_table()->declare(symbol);
		
		procedure.block()->accept(nested_scope_visitor);
		procedure.m_symbol_table = nested_scope_visitor.symbol_table();
		
		// Still add the procedure to the current scope
		this->m_symbol_table->declare(symbol);
	} else
	{
		// This is the first visit we have
		this->m_symbol_table = std::make_shared<::symbol_table>(procedure.identifier(), nullptr);

		// We can reference the current function from the current scope
		this->m_symbol_table->declare(symbol);

		procedure.block()->accept(*this);
	}
}

std::shared_ptr<symbol_table> symbol_table_builder::symbol_table() const
{
	return this->m_symbol_table;
}
