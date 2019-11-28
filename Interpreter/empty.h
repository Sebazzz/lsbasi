#pragma once
#include "ast_common.h"

namespace ast
{
	class empty;	
}


class ast::empty final :
	public ast_node
{
public:
	explicit empty();

	explicit empty(const ast_node& other);

	explicit empty(ast_node&& other);

	void accept(ast_node_visitor& visitor) override;
};

