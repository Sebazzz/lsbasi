#pragma once
#include <memory>
#include "token_type.h"

/**
 * Base class for the abstract syntax tree
 */
class ast_node
{
private:
	token_type m_op;

protected:
	explicit ast_node(token_type token)
		: m_op(token)
	{
	}

public:
	[[nodiscard]] token_type op() const
	{
		return m_op;
	}
};

using ast_ptr = std::shared_ptr<ast_node>;

inline ast_ptr make_ast_ptr(ast_node&& node)
{
	return std::make_shared<ast_node>(node);
}