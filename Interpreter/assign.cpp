#include "pch.h"
#include "assign.h"
#include "ast_node_visitor.h"

using namespace ast;

assign::assign(assign_left_type left, ast_ptr right, token token): ast_node(std::move(token)),
                                                            m_left(std::move(left)),
                                                            m_right(std::move(right))
{
}

assign::assign(assign&& other) noexcept: ast_node(other),
                                         m_left(std::move(other.m_left)),
                                         m_right(std::move(other.m_right))
{
}

assign& assign::operator=(const assign& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_left = other.m_left;
	m_right = other.m_right;
	return *this;
}

assign& assign::operator=(assign&& other) noexcept
{
	if (this == &other)
		return *this;
	m_left = std::move(other.m_left);
	m_right = std::move(other.m_right);

	ast_node::operator =(std::move(other));
	return *this;
}

assign_left_type assign::left() const
{
	return m_left;
}

ast_ptr assign::right() const
{
	return m_right;
}

void assign::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
