#include "pch.h"
#include "block.h"
#include "ast_node_visitor.h"

ast::block::block(var_decl_list var_declarations, routine_decl_list procedure_declarations, compound_ptr compound, token token): ast_node(std::move(token)),
                                                                          m_compound(std::move(compound)),
                                                                          m_var_declarations(std::move(var_declarations)),
                                                                          m_procedure_declarations(std::move(procedure_declarations))
{
}

ast::block::block(block&& other) noexcept: ast_node(std::move(other)),
                                           m_compound(std::move(other.m_compound)),
                                           m_var_declarations(std::move(other.m_var_declarations))
{
}

ast::block& ast::block::operator=(const block& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_compound = other.m_compound;
	m_var_declarations = other.m_var_declarations;
	return *this;
}

ast::block& ast::block::operator=(block&& other) noexcept
{
	if (this == &other)
		return *this;
	m_compound = std::move(other.m_compound);
	m_var_declarations = std::move(other.m_var_declarations);
	ast_node::operator =(std::move(other));
	return *this;
}

const ast::compound_ptr& ast::block::compound() const
{
	return this->m_compound;
}

const ast::var_decl_list& ast::block::var_declarations() const
{
	return this->m_var_declarations;
}

const ast::routine_decl_list& ast::block::procedure_declarations() const
{
	return this->m_procedure_declarations;
}

void ast::block::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
