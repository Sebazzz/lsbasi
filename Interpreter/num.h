#pragma once
#include "ast_node.h"
#include <string>

namespace ast
{
	using num_value = int;

	class num;	
}


class ast::num :
	public ast_node
{
private:
	int m_value;

public:
	num(num_value value);

	num(const num& other) = default;

	num(num&& other) noexcept;

	num& operator=(const num& other);

	num& operator=(num&& other) noexcept;

	~num() = default;

	[[nodiscard]] num_value value() const;
	
	void accept(ast_node_visitor& visitor) override;
};

