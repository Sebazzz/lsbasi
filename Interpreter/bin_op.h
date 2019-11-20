#pragma once
#include "ast_node.h"
#include "token_type.h"

class bin_op final : public ast_node
{
private:
	ast_ptr m_left;
	token_type m_op;
	ast_ptr m_right;

public:
	bin_op(ast_ptr left, token_type op, ast_ptr right);

	bin_op(const bin_op& other) = default;

	bin_op(bin_op&& other) noexcept;

	bin_op& operator=(const bin_op& other);

	bin_op& operator=(bin_op&& other) noexcept;

	~bin_op() = default;

	[[nodiscard]] ast_ptr left() const;

	[[nodiscard]] token_type op() const;

	[[nodiscard]] ast_ptr right() const;

	void accept(ast_node_visitor& visitor) override;
};

