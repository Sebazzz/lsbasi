#include "pch.h"
#include "symbol.h"
#include "procedure.h"
#include "var_decl.h"

symbol::symbol(symbol_type symbol, const symbol_identifier& cs): m_type(symbol), m_identifier(cs)
{
}

bool symbol::operator<(const symbol& rhs) const
{
	return std::tie(this->m_type, this->m_identifier) < std::tie(rhs.m_type, rhs.m_identifier);
}

procedure_symbol::procedure_symbol(ast::procedure_decl_ptr procedure): symbol(symbol_type::procedure, procedure->identifier()), m_procedure(std::move(procedure))
{
}

variable_symbol::variable_symbol(ast::var_decl_ptr variable): symbol(symbol_type::procedure, variable->identifier()), m_var(std::move(variable))
{
}