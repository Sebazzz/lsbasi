#pragma once
#include "ast_common.h"
#include "bin_op.h"
#include "unary_op.h"
#include "num.h"
#include "compound.h"
#include "assign.h"
#include "var.h"
#include "empty.h"
#include "block.h"
#include "program.h"
#include "var_decl.h"
#include "type.h"
#include "procedure.h"

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
	 * Visit a variable - default implementation iterates left/right
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

	/**
	 * Visit a program statement - default implementation is visit of var and block
	 */
	virtual void visit(ast::program& program);

	/**
	 * Visit a program statement - default implementation is visit of var and block
	 */
	virtual void visit(ast::procedure& procedure);

	/**
	 * Visit a variable declaration - default implementation visits inner type
	 */
	virtual void visit(ast::var_decl& var_decl);

	/**
	 * Visit a type - default implementation is no-op
	 */
	virtual void visit(ast::type& type_ref);

	/**
	 * Visit a block - default implementation visits var declarations and compound
	 */
	virtual void visit(ast::block& block);
	
};

