#pragma once
#include "ast_common.h"

namespace ast
{
	class num;	
}

struct memory_contents;

class ast::num :
	public ast_node
{
private:
	expression_value m_value;
	builtin_type m_type;

public:
	num(double value, token token);
	num(int value, token token);

	num(const num& other) = default;

	num(num&& other) noexcept;

	num& operator=(const num& other);

	num& operator=(num&& other) noexcept;

	~num() = default;

	[[nodiscard]] std::wstring val_to_string() const;
	[[nodiscard]] expression_value value() const;
	[[nodiscard]] builtin_type type() const;
	
	void accept(ast_node_visitor& visitor) override;
};

