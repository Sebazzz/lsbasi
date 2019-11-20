#include "num.h"
#include "ast_node_visitor.h"

num::num(const num_value value): ast_node(token_type::integer), m_value(value)
{
}

num::num(num&& other) noexcept: ast_node(other),
                                m_value(other.m_value)
{
}

num& num::operator=(const num& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_value = other.m_value;
	return *this;
}

num& num::operator=(num&& other) noexcept
{
	if (this == &other)
		return *this;
	m_value = other.m_value;
	ast_node::operator =(other);
	return *this;
}

num_value num::value() const
{
	return m_value;
}

void num::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
