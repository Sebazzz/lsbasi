#pragma once
#include "ast_common.h"
#include "bin_op.h"
#include "unary_op.h"
#include "num.h"
#include "compound.h"
#include "assign.h"
#include "assignment_target.h"
#include "empty.h"
#include "block.h"
#include "program.h"
#include "var_decl.h"
#include "type.h"
#include "if_then_else.h"
#include "routine.h"
#include "routine_call.h"

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
	virtual void visit(ast::assignment_target& variable);

	/**
	 * Visit a empty statement - default implementation is no-op
	 */
	virtual void visit(ast::empty& empty_statement);

	/**
	 * Visit a program statement - default implementation is visit of assignment_target and block
	 */
	virtual void visit(ast::program& program);

	/**
	 * Visit a routine statement - default implementation is visit of params and block
	 */
	virtual void visit(ast::routine& procedure);

	/**
	 * Visit a procedure call - default implementation is no-op
	 */
	virtual void visit(ast::routine_call& procedure_call);

	/**
	 * Visit a variable declaration - default implementation visits inner type
	 */
	virtual void visit(ast::var_decl& var_decl);

	/**
	 * Visit a type - default implementation is no-op
	 */
	virtual void visit(ast::type& type_ref);

	/**
	 * Visit a block - default implementation visits assignment_target declarations and compound
	 */
	virtual void visit(ast::block& block);

	/**
	 * Visit an if/then/else statement - default implementation visits the test expression and true/false statement
	 */
	virtual void visit(ast::if_then_else& if_then_else);
};

