#include "ast_node_visitor.h"

void ast_node_visitor::visit(ast_node& ast_node)
{
	ast_node.accept(*this);
}

void ast_node_visitor::visit(bin_op& node)
{
	// For derived classes to implement. By default, traversed into the node.
	
	node.left()->accept(*this);
	node.right()->accept(*this);
}

void ast_node_visitor::visit(unary_op& node)
{
	// For derived classes to implement. By default, traversed into the node.
	node.expr()->accept(*this);
}

void ast_node_visitor::visit(num&)
{
	// For derived classes to implement
}
