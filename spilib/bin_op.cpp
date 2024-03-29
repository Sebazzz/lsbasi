#include "pch.h"
#include "bin_op.h"
#include "ast_node_visitor.h"

using namespace ast;

bin_op::bin_op(ast_ptr left, token_type op, ast_ptr right, token token): ast_node(std::move(token)),
                                                            m_left(std::move(left)),
                                                            m_op(op),
                                                            m_right(std::move(right))
{
}

bin_op::bin_op(bin_op&& other) noexcept: ast_node(other),
                                         m_left(std::move(other.m_left)),
                                         m_op(other.m_op),
                                         m_right(std::move(other.m_right))
{
}

bin_op& bin_op::operator=(const bin_op& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_left = other.m_left;
	m_op = other.m_op;
	m_right = other.m_right;
	return *this;
}

bin_op& bin_op::operator=(bin_op&& other) noexcept
{
	if (this == &other)
		return *this;
	m_left = std::move(other.m_left);
	m_op = other.m_op;
	m_right = std::move(other.m_right);

	ast_node::operator =(std::move(other));
	return *this;
}

ast_ptr bin_op::left() const
{
	return m_left;
}

token_type bin_op::op() const
{
	return m_op;
}

ast_ptr bin_op::right() const
{
	return m_right;
}

void bin_op::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
