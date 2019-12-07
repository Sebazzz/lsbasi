#include "pch.h"
#include "routine.h"
#include "symbol_table.h"
#include "routine_symbol.h"
#include "type_symbol.h"

routine_symbol::routine_symbol(const symbol_identifier& identifier): symbol(symbol_type::procedure, identifier)
{
}

const ast::routine& user_defined_routine_symbol::procedure() const
{
	return this->m_procedure;
}

symbol_type_ptr<type_symbol> user_defined_routine_symbol::return_type() const
{
	return this->m_procedure.return_type()->type_symbol();
}

bool user_defined_routine_symbol::is_function() const
{
	return this->m_procedure.is_function();
}

const ast::routine_param_list& user_defined_routine_symbol::params() const
{
	return this->m_procedure.params();
}

::symbol_table& user_defined_routine_symbol::symbol_table() const
{
	return this->m_procedure.symbol_table();
}

builtin_routine_symbol::builtin_routine_symbol(const symbol_identifier& identifier, ::symbol_table* runtime_symbol_table) : routine_symbol(identifier), m_runtime_symbol_table(runtime_symbol_table)
{
}

ast::type_ptr builtin_routine_symbol::get_builtin_type(ast::builtin_type type_spec) const
{
	const auto& identifier = this->m_runtime_symbol_table->get(type_spec)->identifier();
	return ast::make_ast_ptr<ast::type>(identifier, token(token_type::identifier, identifier));
}

void builtin_routine_symbol::register_param(const symbol_identifier& identifier, ast::builtin_type builtin_type)
{
	this->m_param_list.push_back(ast::make_ast_ptr<ast::var_decl>(identifier, this->get_builtin_type(builtin_type), token(token_type::identifier, identifier, line_info {-1,-1})));
}

const ast::routine_param_list& builtin_routine_symbol::params() const
{
	return this->m_param_list;
}

::symbol_table& builtin_routine_symbol::symbol_table() const
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

std::wstring routine_symbol::to_string() const
{
	return (this->is_function() ? L"function " : L"procedure ") + this->identifier();
}

user_defined_routine_symbol::user_defined_routine_symbol(ast::routine& procedure): routine_symbol(procedure.identifier()), m_procedure(procedure)
{
}

