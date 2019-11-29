#include "pch.h"
#include "var_decl.h"
#include "ast_node_visitor.h"

ast::var_decl::var_decl(var_identifier identifier, ast::type_ptr type): ast_node(token_type::var_decl),
                                                                   m_identifier(std::move(identifier)),
                                                                   m_type(type)
{
}

ast::var_decl::var_decl(var_decl&& other) noexcept: ast_node(std::move(other)),
                                                    m_identifier(std::move(other.m_identifier)),
                                                    m_type(other.m_type)
{
}

ast::var_decl& ast::var_decl::operator=(const var_decl& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_identifier = other.m_identifier;
	m_type = other.m_type;
	return *this;
}

ast::var_decl& ast::var_decl::operator=(var_decl&& other) noexcept
{
	if (this == &other)
		return *this;
	ast_node::operator =(std::move(other));
	m_identifier = std::move(other.m_identifier);
	m_type = other.m_type;
	return *this;
}

const ast::var_identifier& ast::var_decl::identifier() const
{
	return this->m_identifier;
}

ast::type_ptr ast::var_decl::type() const
{
	return this->m_type;
}

void ast::var_decl::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
