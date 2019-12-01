#include "pch.h"
#include "type.h"
#include "ast_node_visitor.h"

using namespace ast;

type::type(type_identifier identifier, token token): ast_node(std::move(token)), m_identifier(std::move(identifier))
{
}

type::type(type&& other) noexcept: ast_node(other),
                                m_identifier(other.m_identifier)
{
}

type& type::operator=(const type& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_identifier = other.m_identifier;
	return *this;
}

type& type::operator=(type&& other) noexcept
{
	if (this == &other)
		return *this;
	m_identifier = other.m_identifier;
	ast_node::operator =(other);
	return *this;
}

const type_identifier& type::identifier() const
{
	return m_identifier;
}

void type::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
