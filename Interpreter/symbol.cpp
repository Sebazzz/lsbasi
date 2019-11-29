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

builtin_type_symbol::builtin_type_symbol(ast::builtin_type type): symbol(symbol_type::type, var_type_to_string(type)), m_type(type)
{
}

ast::builtin_type builtin_type_symbol::type() const
{
	return this->m_type;
}

symbol_identifier builtin_type_symbol::var_type_to_string(ast::builtin_type type)
{
	switch (type)
	{
	case ast::builtin_type::integer:
		return L"INTEGER";
	case ast::builtin_type::real:
		return L"REAL";
	default:
		throw std::logic_error("Unknown built-in variable type");
		;
	}
}

std::wstring builtin_type_symbol::to_string() const
{
	return L"builtin " + this->identifier();
}

procedure_symbol::procedure_symbol(ast::procedure& procedure): symbol(symbol_type::procedure, procedure.identifier()), m_procedure(procedure)
{
}

const ast::procedure& procedure_symbol::procedure() const
{
	return this->m_procedure;
}

std::wstring procedure_symbol::to_string() const
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

std::wstring variable_symbol::to_string() const
{
	return L"variable " + this->m_var.identifier();
}
