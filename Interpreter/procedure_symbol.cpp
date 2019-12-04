#include "pch.h"
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

std::wstring procedure_symbol::to_string() const
{
	return L"procedure " + this->identifier();
}

user_defined_procedure_symbol::user_defined_procedure_symbol(ast::procedure& procedure): procedure_symbol(procedure.identifier()), m_procedure(procedure)
{
}