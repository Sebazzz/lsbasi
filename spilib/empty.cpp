#include "pch.h"
#include "empty.h"
#include "ast_node_visitor.h"

ast::empty::empty(token token): ast_node(std::move(token))
{
}

ast::empty::empty(const ast_node& other): ast_node(other)
{
}

ast::empty::empty(ast_node&& other): ast_node(other)
{
}

void ast::empty::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
