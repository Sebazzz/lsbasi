#pragma once
#include "ast_node.h"
#include "bin_op.h"
#include "num.h"

class ast_node_visitor
{

public:
	virtual ~ast_node_visitor() = default;
	
	virtual void visit(ast_node& ast_node);
	virtual void visit(bin_op& binaryOperator);
	virtual void visit(num& number);
};

