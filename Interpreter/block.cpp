#include "block.h"
#include "ast_node_visitor.h"

ast::block::block(var_decl_list var_declarations, compound_ptr compound): ast_node(token_type::var_decl),
                                                                          m_compound(std::move(compound)),
                                                                          m_var_declarations(
	                                                                          std::move(var_declarations))
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

void ast::block::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
