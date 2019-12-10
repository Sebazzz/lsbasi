#include "pch.h"

using namespace ast;

ast_node::ast_node(token token): m_token(std::move(token))
{
}

const token& ast_node::get_token() const
{
	return m_token;
}

line_info ast_node::get_line_info() const
{
	return m_token.position();
}

ast_node::ast_node(const ast_node& other) = default;

ast_node::ast_node(ast_node&& other) noexcept: m_token(std::move(other.m_token))
{
}

ast_node& ast_node::operator=(const ast_node& other)
{
	if (this == &other)
		return *this;
	m_token = other.m_token;
	return *this;
}

ast_node& ast_node::operator=(ast_node&& other) noexcept
{
	if (this == &other)
		return *this;
	m_token = other.m_token;
	return *this;
}
