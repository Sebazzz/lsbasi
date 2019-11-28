#include "pch.h"
#include "compound.h"
#include "ast_node_visitor.h"

ast::compound::compound(ast::statement_list ast_nodes): ast_node(token_type::begin),
                                                                            m_statements(std::move(ast_nodes))
{
}

ast::compound::compound(const compound& other): ast_node(other),
                                                m_statements(other.m_statements)
{
}

ast::compound::compound(compound&& other) noexcept: ast_node(std::move(other)),
                                                    m_statements(std::move(other.m_statements))
{
}

ast::compound& ast::compound::operator=(const compound& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_statements = other.m_statements;
	return *this;
}

ast::compound& ast::compound::operator=(compound&& other) noexcept
{
	if (this == &other)
		return *this;
	m_statements = std::move(other.m_statements);
	ast_node::operator =(std::move(other));
	return *this;
}

const ast::statement_list& ast::compound::statements() const
{
	return this->m_statements;
}

void ast::compound::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
