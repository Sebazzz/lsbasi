#include "empty.h"

ast::empty::empty(): ast_node(token_type::eof)
{
}

ast::empty::empty(const ast_node& other): ast_node(other)
{
}

ast::empty::empty(ast_node&& other): ast_node(other)
{
}
