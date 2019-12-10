#pragma once
#include "ast_common.h"

namespace ast
{
	class block;
	
	struct if_block
	{
		ast_node_ptr<block> block_ptr;
		ast_ptr ast_ptr;
	};
	
	class if_then_else;
}

class ast::if_then_else final : public ast_node
{
private:
	ast_ptr m_test_expression;
	if_block m_true_block;
	if_block m_false_block;

public:
	if_then_else(ast_ptr test_expression, if_block true_block, if_block false_block, token token);

	if_then_else(const if_then_else& other);

	if_then_else(if_then_else&& other) noexcept;

	if_then_else& operator=(const if_then_else& other);

	if_then_else& operator=(if_then_else&& other) noexcept;

	~if_then_else() = default;

	[[nodiscard]] const ast_ptr& test_expression() const;
	[[nodiscard]] const if_block& true_block() const;
	[[nodiscard]] const if_block& false_block() const;
	
	void accept(ast_node_visitor& visitor) override;
};
