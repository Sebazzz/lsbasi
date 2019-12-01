#include "pch.h"
#include "symbol.h"
#include "symbol_table_builder.h"
#include "type_symbol.h"

template<class T>
void verify_exists(symbol_table& symbol_table_instance, const symbol_identifier& identifier)
{
	// Throws on failure
	try
	{
		if (!symbol_table_instance.get<T>(identifier))
		{
			throw interpret_except("symbol_type::get will throw");
		}
	} catch (interpret_except& e)
	{
		throw interpret_except(std::string("Unable to resolve symbol of type '") + typeid(T).name() + "': " + e.what());
	}
}

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
	verify_exists<variable_symbol>(*this->m_symbol_table, variable.identifier());
}

void symbol_table_builder::visit(ast::type& type_ref)
{
	verify_exists<type_symbol>(*this->m_symbol_table, type_ref.identifier());
}

void symbol_table_builder::visit(ast::procedure_call& procedure_call)
{
	// Verify that we can actually resolve this procedure
	// FIXME: this assumes that procedure has been declared earlier in the file at the moment
	verify_exists<procedure_symbol>(*this->m_symbol_table, procedure_call.procedure_identifier());
	
	// Verify each of the parameters
	try
	{
		for (const auto& param_arg : procedure_call.args())
		{
			param_arg->accept(*this);
		}
	} catch (interpret_except& e)
	{
		throw interpret_except(L"In call to procedure '" + procedure_call.procedure_identifier() + L"' unexpected error found: " + string_to_wstring(e.what()));
	}
}

void symbol_table_builder::visit(ast::var_decl& var_decl)
{
	this->ensure_symbol_table();

	const auto symbol = make_symbol_ptr<variable_symbol>(var_decl);
	this->m_symbol_table->declare(symbol);

	ast_node_visitor::visit(var_decl);
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
	if (!this->m_symbol_table)
	{
		// We should have a symbol table from the global scope or parent procedure. This is an invalid state.
		throw interpret_except("Attempting to visit procedure without a global scope or parent procedure scope");
	}

	// In a nested visitor, we are visiting ourself. This is the first visit.
	// The base implementation will iterate the parameters and finally the body.
	if (this->m_symbol_table.get() == procedure.m_symbol_table.get())
	{
		ast_node_visitor::visit(procedure);
		return;
	}

	const auto symbol = make_symbol_ptr<procedure_symbol>(procedure);

	// We found a (nested) procedure. New scope.
	symbol_table_builder nested_scope_visitor(procedure.identifier(), this->m_symbol_table.get());
	nested_scope_visitor.symbol_table()->declare(symbol);
	procedure.m_symbol_table = nested_scope_visitor.symbol_table();
	
	procedure.accept(nested_scope_visitor);

	// ... add the procedure to the current scope so it can be referenced
	this->m_symbol_table->declare(symbol);
}

std::shared_ptr<symbol_table> symbol_table_builder::symbol_table() const
{
	return this->m_symbol_table;
}
