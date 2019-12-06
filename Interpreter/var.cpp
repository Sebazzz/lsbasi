#include "pch.h"
#include "var.h"
#include "ast_node_visitor.h"

using namespace ast;

var::var(var_identifier identifier, token token): ast_node(std::move(token)), m_identifier(std::move(identifier))
{
}

var::var(var&& other) noexcept: ast_node(other),
                                m_identifier(other.m_identifier)
{
}

var& var::operator=(const var& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_identifier = other.m_identifier;
	return *this;
}

var& var::operator=(var&& other) noexcept
{
	if (this == &other)
		return *this;
	m_identifier = other.m_identifier;
	ast_node::operator =(other);
	return *this;
}

const var_identifier& var::identifier() const
{
	return m_identifier;
}

void var::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}

symbol_type_ptr<variable_symbol> var::variable_symbol() const
{
	if (!this->m_variable_symbol)
	{
		throw internal_interpret_except(L"Variable symbol was not made available in semantic analysis step");
	}
	
	return this->m_variable_symbol;
}
