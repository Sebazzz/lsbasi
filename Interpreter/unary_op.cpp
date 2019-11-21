#include "unary_op.h"
#include "ast_node_visitor.h"

unary_op::unary_op(token_type op, ast_ptr expr): ast_node(op),
                                                            m_op(op),
                                                            m_expr(std::move(expr))
{
}

unary_op::unary_op(unary_op&& other) noexcept: ast_node(other),
                                         m_op(other.m_op),
                                         m_expr(std::move(other.m_expr))
{
}

unary_op& unary_op::operator=(const unary_op& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_op = other.m_op;
	m_expr = other.m_expr;
	return *this;
}

unary_op& unary_op::operator=(unary_op&& other) noexcept
{
	if (this == &other)
		return *this;
	m_op = other.m_op;
	m_expr = std::move(other.m_expr);

	ast_node::operator =(std::move(other));
	return *this;
}

token_type unary_op::op() const
{
	return m_op;
}

ast_ptr unary_op::expr() const
{
	return m_expr;
}

void unary_op::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
