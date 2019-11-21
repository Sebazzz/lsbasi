#pragma once

#include "ast_node.h"
#include "token_type.h"

class unary_op final : public ast_node
{
private:
	token_type m_op;
	ast_ptr m_expr;

public:
	unary_op(token_type op, ast_ptr expr);

	unary_op(const unary_op& other) = default;

	unary_op(unary_op&& other) noexcept;

	unary_op& operator=(const unary_op& other);

	unary_op& operator=(unary_op&& other) noexcept;

	~unary_op() = default;

	[[nodiscard]] token_type op() const;

	[[nodiscard]] ast_ptr expr() const;

	void accept(ast_node_visitor& visitor) override;
};
