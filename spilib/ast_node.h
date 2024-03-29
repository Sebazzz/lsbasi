#pragma once
#include <memory>
#include "token_type.h"
#include "token.h"

class ast_node_visitor;

namespace ast
{
	class ast_node;

	/* Type for pointer to AST node */
	using ast_ptr = std::shared_ptr<ast_node>;

	template<typename T>
	using ast_node_ptr = std::shared_ptr<T>;

	/* Helper function for making a pointer to AST node */
	template <class T, class... TArgs>
	ast_node_ptr<T> make_ast_ptr(TArgs&&... args)
	{
		static_assert(std::is_convertible<T*, ast_node*>::value, "This template is only suitable for ast::ast_node");
		return std::make_shared<T>(std::forward<TArgs>(args)...);
	}
}

/**
 * Base class for the abstract syntax tree
 */
class ast::ast_node
{
private:
	token m_token;

protected:
	explicit ast_node(token token);

public:
	[[nodiscard]] const token& get_token() const;
	[[nodiscard]] line_info get_line_info() const;

	virtual ~ast_node() = default;

	ast_node(const ast_node& other);

	ast_node(ast_node&& other) noexcept;

	ast_node& operator=(const ast_node& other);

	ast_node& operator=(ast_node&& other) noexcept;

	virtual void accept(ast_node_visitor& visitor) = 0;
};

