#include "ast_node.h"

ast_node::ast_node(token_type token): m_token(token)
{
}

token_type ast_node::get_token() const
{
	return m_token;
}

ast_node::ast_node(const ast_node& other): m_token(other.m_token)
{
}

ast_node::ast_node(ast_node&& other) noexcept: m_token(other.m_token)
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
