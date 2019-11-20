#include "ast_node_visitor.h"

void ast_node_visitor::visit(ast_node& ast_node)
{
	ast_node.accept(*this);
}

void ast_node_visitor::visit(bin_op&)
{
	// For derived classes to implement
}

void ast_node_visitor::visit(num&)
{
	// For derived classes to implement
}
