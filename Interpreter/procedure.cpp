#include "pch.h"
#include "procedure.h"
#include "ast_node_visitor.h"

ast::procedure::procedure(procedure_identifier id, block_ptr block): ast_node(token_type::procedure),
                                                               m_identifier(std::move(id)),
                                                               m_block(std::move(block))
{
}

ast::procedure::procedure(procedure&& other) noexcept: ast_node(std::move(other)),
                                                 m_identifier(std::move(other.m_identifier)),
                                                 m_block(std::move(other.m_block))
{
}

ast::procedure& ast::procedure::operator=(const procedure& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_identifier = other.m_identifier;
	m_block = other.m_block;
	return *this;
}

ast::procedure& ast::procedure::operator=(procedure&& other) noexcept
{
	if (this == &other)
		return *this;
	m_identifier = std::move(other.m_identifier);
	m_block = std::move(other.m_block);
	ast_node::operator =(std::move(other));
	return *this;
}

const ast::procedure_identifier& ast::procedure::identifier() const
{
	return this->m_identifier;
}

const ast::block_ptr& ast::procedure::block() const
{
	return this->m_block;
}

void ast::procedure::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
