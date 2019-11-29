#include "pch.h"
#include "symbol.h"
#include "procedure.h"
#include "var_decl.h"

symbol::symbol(symbol_type symbol, const symbol_identifier& cs): m_type(symbol), m_identifier(cs)
{
}

const std::wstring& symbol::identifier() const
{
	return this->m_identifier;
}

bool symbol::operator<(const symbol& rhs) const
{
	if (this->m_type < rhs.m_type)
		return true;
	if (rhs.m_type < this->m_type)
		return false;

	const case_insensitive_string_comparer comparer;
	return comparer(this->m_identifier, rhs.m_identifier);
}

procedure_symbol::procedure_symbol(ast::procedure& procedure): symbol(symbol_type::procedure, procedure.identifier()), m_procedure(procedure)
{
}

const ast::procedure& procedure_symbol::procedure() const
{
	return this->m_procedure;
}

std::wstring procedure_symbol::to_string()
{
	return L"procedure " + this->m_procedure.identifier();
}

variable_symbol::variable_symbol(ast::var_decl& variable): symbol(symbol_type::procedure, variable.identifier()), m_var(variable)
{
}

const ast::var_decl& variable_symbol::variable() const
{
	return this->m_var;
}

std::wstring variable_symbol::to_string()
{
	return L"variable " + this->m_var.identifier();
}
