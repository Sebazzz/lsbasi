#include "pch.h"
#include "procedure_symbol.h"

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

