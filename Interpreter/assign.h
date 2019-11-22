#pragma once
#include "ast_node.h"
#include "token_type.h"
#include "var.h"

namespace ast
{
	using assign_left_type = ast_node_ptr<var>;
	
	class assign;
}

class ast::assign final : public ast_node
{
private:
	assign_left_type m_left;
	ast_ptr m_right;

public:
	assign(assign_left_type left, ast_ptr right);

	assign(const assign& other) = default;

	assign(assign&& other) noexcept;

	assign& operator=(const assign& other);

	assign& operator=(assign&& other) noexcept;

	~assign() = default;

	[[nodiscard]] assign_left_type left() const;

	[[nodiscard]] ast_ptr right() const;

	void accept(ast_node_visitor& visitor) override;
};

