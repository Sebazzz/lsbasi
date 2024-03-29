#include "pch.h"
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

void ast_node_visitor::visit(ast::assign& assign)
{
	assign.left()->accept(*this);
	assign.right()->accept(*this);
}

void ast_node_visitor::visit(ast::assignment_target&)
{
	// For derived classes to implement
}

void ast_node_visitor::visit(ast::empty&)
{
	// For derived classes to implement
}

void ast_node_visitor::visit(ast::program& program)
{
	program.block()->accept(*this);
}

void ast_node_visitor::visit(ast::routine& procedure)
{
	for (auto& param_decl : procedure.params())
	{
		param_decl->accept(*this);
	}

	if (procedure.is_function())
	{
		procedure.return_type()->accept(*this);
	}
	
	procedure.block()->accept(*this);
}

void ast_node_visitor::visit(ast::routine_call&)
{
	// For derived classes to implement
}

void ast_node_visitor::visit(ast::var_decl& var_decl)
{
	var_decl.type()->accept(*this);
}

void ast_node_visitor::visit(ast::type&)
{
	// For derived classes to implement
}

void ast_node_visitor::visit(ast::block& block)
{
	for (auto& var_decl : block.var_declarations())
	{
		var_decl->accept(*this);
	}

	for (auto& proc_decl : block.procedure_declarations())
	{
		proc_decl->accept(*this);
	}

	block.compound()->accept(*this);
}

void ast_node_visitor::visit(ast::if_then_else& if_then_else)
{
	if_then_else.test_expression()->accept(*this);

	if (if_then_else.true_block().ast_ptr) if_then_else.true_block().ast_ptr->accept(*this);
	if (if_then_else.true_block().block_ptr) if_then_else.true_block().block_ptr->accept(*this);
	if (if_then_else.false_block().ast_ptr) if_then_else.false_block().ast_ptr->accept(*this);
	if (if_then_else.false_block().block_ptr) if_then_else.false_block().block_ptr->accept(*this);
}
