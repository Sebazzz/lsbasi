#include "pch.h"
#include "procedure.h"
#include "symbol_table.h"
#include "procedure_symbol.h"

procedure_symbol::procedure_symbol(const symbol_identifier& identifier): symbol(symbol_type::procedure, identifier)
{
}

const ast::procedure& user_defined_procedure_symbol::procedure() const
{
	return this->m_procedure;
}

const ast::procedure_param_list& user_defined_procedure_symbol::params() const
{
	return this->m_procedure.params();
}

::symbol_table& user_defined_procedure_symbol::symbol_table() const
{
	return this->m_procedure.symbol_table();
}

builtin_procedure_symbol::builtin_procedure_symbol(const symbol_identifier& identifier, ::symbol_table* runtime_symbol_table) : procedure_symbol(identifier), m_runtime_symbol_table(runtime_symbol_table)
{
}

ast::type_ptr builtin_procedure_symbol::get_builtin_type(ast::builtin_type type_spec) const
{
	const auto& identifier = this->m_runtime_symbol_table->get(type_spec)->identifier();
	return ast::make_ast_node_ptr<ast::type>(identifier, token(token_type::identifier, identifier));
}

void builtin_procedure_symbol::register_param(const symbol_identifier& identifier, ast::builtin_type builtin_type)
{
	this->m_param_list.push_back(ast::make_ast_node_ptr<ast::var_decl>(identifier, this->get_builtin_type(builtin_type), token(token_type::identifier, identifier, line_info {-1,-1})));
}

const ast::procedure_param_list& builtin_procedure_symbol::params() const
{
	return this->m_param_list;
}

::symbol_table& builtin_procedure_symbol::symbol_table() const
{
	// The symbol table is only there so that the interpreter can bind the symbols to the parameters
	if (!this->m_symbol_table)
	{
		this->m_symbol_table = std::make_unique<::symbol_table>(L"PROCEDURE: " + this->identifier(), this->m_runtime_symbol_table);

		for (auto param_ptr : this->m_param_list)
		{
			auto& param = *param_ptr;
			const auto param_symbol = make_symbol_ptr<variable_symbol>(param);
			this->m_symbol_table->declare(param_symbol,param.get_line_info());
		}
	}

	return *this->m_symbol_table;
}

std::wstring procedure_symbol::to_string() const
{
	return L"procedure " + this->identifier();
}

user_defined_procedure_symbol::user_defined_procedure_symbol(ast::procedure& procedure): procedure_symbol(procedure.identifier()), m_procedure(procedure)
{
}

