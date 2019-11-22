#pragma once
#include "ast_node.h"

namespace ast
{
	class empty;	
}


class ast::empty :
	public ast_node
{
public:
	explicit empty();

	explicit empty(const ast_node& other);

	explicit empty(ast_node&& other);
};

