#include "pch.h"
#include "routine.h"
#include "ast_node_visitor.h"

ast::routine::routine(procedure_identifier id, routine_param_list params, block_ptr block, token token): ast_node(std::move(token)),
                                                               m_identifier(std::move(id)),
                                                               m_params(std::move(params)),
                                                               m_block(std::move(block))
{
}

ast::routine::routine(procedure_identifier id, routine_param_list params, type_ptr return_type, block_ptr block, token token): ast_node(std::move(token)),
                                                               m_identifier(std::move(id)),
                                                               m_params(std::move(params)),
                                                               m_return_type(std::move(return_type)),
                                                               m_block(std::move(block))
{
}

ast::routine::routine(routine&& other) noexcept: ast_node(std::move(other)),
                                                 m_identifier(std::move(other.m_identifier)),
                                                 m_params(std::move(other.m_params)),
                                                 m_return_type(std::move(other.m_return_type)),
                                                 m_block(std::move(other.m_block))
{
}

ast::routine& ast::routine::operator=(const routine& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_identifier = other.m_identifier;
	m_params = other.m_params;
	m_return_type = other.m_return_type;
	m_block = other.m_block;
	return *this;
}

ast::routine& ast::routine::operator=(routine&& other) noexcept
{
	if (this == &other)
		return *this;
	m_identifier = std::move(other.m_identifier);
	m_params = std::move(other.m_params);
	m_return_type = std::move(other.m_return_type);
	m_block = std::move(other.m_block);
	ast_node::operator =(std::move(other));
	return *this;
}

const ast::procedure_identifier& ast::routine::identifier() const
{
	return this->m_identifier;
}

const ast::type_ptr& ast::routine::return_type() const
{
	return this->m_return_type;
}

bool ast::routine::is_function() const
{
	return !!this->m_return_type;
}

const ast::block_ptr& ast::routine::block() const
{
	return this->m_block;
}

const ast::routine_param_list& ast::routine::params() const
{
	return this->m_params;
}

void ast::routine::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}

symbol_table& ast::routine::symbol_table() const
{
	if (!this->m_symbol_table)
	{
		throw internal_interpret_except(L"The symbol table has not been initialized: " + this->identifier(), this->get_line_info());
	}
	
	return *this->m_symbol_table.get();
}
