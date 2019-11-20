#include "ast_node.h"

ast_node::ast_node(token_type token): m_op(token)
{
}

token_type ast_node::op() const
{
	return m_op;
}
