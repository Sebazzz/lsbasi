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
	explicit ast_node(token_type token);

public:
	[[nodiscard]] token_type op() const;
};

/* Type for pointer to AST node */
using ast_ptr = std::shared_ptr<ast_node>;

/* Helper function for making a pointer to AST node */
template <class T, class... TArgs>
ast_ptr make_ast_ptr(TArgs&&... args)
{
	return std::make_shared<T>(args);
}