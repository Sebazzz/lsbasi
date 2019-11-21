#pragma once
#include "ast_node.h"
#include "bin_op.h"
#include "unary_op.h"
#include "num.h"

class ast_node_visitor
{

public:
	virtual ~ast_node_visitor() = default;
	
	virtual void visit(ast::ast_node& ast_node);
	virtual void visit(ast::bin_op& binaryOperator);
	virtual void visit(ast::unary_op& unaryOperator);
	virtual void visit(ast::num& number);
};

