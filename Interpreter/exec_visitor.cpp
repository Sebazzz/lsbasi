#include "exec_visitor.h"

exec_visitor::exec_visitor() = default;

void exec_visitor::visit(ast::ast_node& node)
{
	ast_node_visitor::visit(node);
}
