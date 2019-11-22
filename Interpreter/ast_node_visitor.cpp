#include "ast_node_visitor.h"

void ast_node_visitor::visit(ast::ast_node& ast_node)
{
	ast_node.accept(*this);
}

void ast_node_visitor::visit(ast::bin_op& node)
{
	// For derived classes to implement. By default, traversed into the node.
	
	node.left()->accept(*this);
	node.right()->accept(*this);
}

void ast_node_visitor::visit(ast::unary_op& node)
{
	// For derived classes to implement. By default, traversed into the node.
	node.expr()->accept(*this);
}

void ast_node_visitor::visit(ast::num&)
{
	// For derived classes to implement
}

void ast_node_visitor::visit(ast::compound& compound)
{
	for (auto& statement : compound.statements())
	{
		statement->accept(*this);
	}
}

void ast_node_visitor::visit(ast::assign&)
{
	// For derived classes to implement
}

void ast_node_visitor::visit(ast::var&)
{
	// For derived classes to implement
}

void ast_node_visitor::visit(ast::empty&)
{
	// For derived classes to implement
}
