#include "pch.h"
#include "program.h"
#include "ast_node_visitor.h"

ast::program::program(program_identifier id, block_ptr block, token token): ast_node(std::move(token)),
                                                               m_identifier(std::move(id)),
                                                               m_block(std::move(block))
{
}

ast::program::program(program&& other) noexcept: ast_node(std::move(other)),
                                                 m_identifier(std::move(other.m_identifier)),
                                                 m_block(std::move(other.m_block))
{
}

ast::program& ast::program::operator=(const program& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_identifier = other.m_identifier;
	m_block = other.m_block;
	return *this;
}

ast::program& ast::program::operator=(program&& other) noexcept
{
	if (this == &other)
		return *this;
	m_identifier = std::move(other.m_identifier);
	m_block = std::move(other.m_block);
	ast_node::operator =(std::move(other));
	return *this;
}

const ast::program_identifier& ast::program::identifier() const
{
	return this->m_identifier;
}

const ast::block_ptr& ast::program::block() const
{
	return this->m_block;
}

void ast::program::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}

symbol_table& ast::program::symbol_table() const
{
	if (!this->m_symbol_table)
	{
		throw interpret_except(L"The symbol table has not been initialized: " + this->identifier());
	}
	
	return *this->m_symbol_table.get();
}
