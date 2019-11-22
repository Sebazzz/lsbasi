#pragma once
#include "ast_node.h"
#include "bin_op.h"
#include "unary_op.h"
#include "num.h"
#include "compound.h"
#include "assign.h"
#include "var.h"
#include "empty.h"

class ast_node_visitor
{

public:
	virtual ~ast_node_visitor() = default;

	/**
	 * Visit a variable - default implementation calls accept on the ast_node (invoking the double dispatch)
	 */
	virtual void visit(ast::ast_node& ast_node);

	/**
	 * Visit a variable - default implementation is no-op
	 */
	virtual void visit(ast::bin_op& node);

	/**
	 * Visit a variable - default implementation is no-op
	 */
	virtual void visit(ast::unary_op& node);

	/**
	 * Visit a variable - default implementation is no-op
	 */
	virtual void visit(ast::num& number);

	/**
	 * Visit a variable - default implementation iterates child statements
	 */
	virtual void visit(ast::compound& compound);

	/**
	 * Visit a variable - default implementation is no-op
	 */
	virtual void visit(ast::assign& assign);

	/**
	 * Visit a variable - default implementation is no-op
	 */
	virtual void visit(ast::var& variable);

	/**
	 * Visit a empty statement - default implementation is no-op
	 */
	virtual void visit(ast::empty& empty_statement);
	
};

