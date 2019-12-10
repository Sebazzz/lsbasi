#include "pch.h"
#include "if_then_else.h"
#include <utility>
#include "ast_node_visitor.h"

ast::if_then_else::if_then_else(ast_ptr test_expression, if_block true_block, if_block false_block, token token):
	ast_node(
		std::move(	token)),
		m_test_expression(std::move(test_expression)),
		m_true_block(std::move(true_block)),
		m_false_block(std::move(false_block))
{
}

ast::if_then_else::if_then_else(const if_then_else& other) = default;

ast::if_then_else::if_then_else(if_then_else&& other) noexcept: ast_node(std::move(other)),
                                                                m_test_expression(std::move(other.m_test_expression)),
                                                                m_true_block(std::move(other.m_true_block)),
                                                                m_false_block(std::move(other.m_false_block))
{
}

ast::if_then_else& ast::if_then_else::operator=(const if_then_else& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_test_expression = other.m_test_expression;
	m_true_block = other.m_true_block;
	m_false_block = other.m_false_block;
	return *this;
}

ast::if_then_else& ast::if_then_else::operator=(if_then_else&& other) noexcept
{
	if (this == &other)
		return *this;
	m_test_expression = std::move(other.m_test_expression);
	m_true_block = std::move(other.m_true_block);
	m_false_block = std::move(other.m_false_block);
	ast_node::operator =(std::move(other));
	return *this;
}

const ast::ast_ptr& ast::if_then_else::test_expression() const
{
	return this->m_test_expression;
}

const ast::if_block& ast::if_then_else::true_block() const
{
	return this->m_true_block;
}

const ast::if_block& ast::if_then_else::false_block() const
{
	return this->m_false_block;
}

void ast::if_then_else::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
