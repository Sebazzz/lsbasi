#pragma once
#include "ast_common.h"
#include "ast_common.h"

namespace ast
{
	class var;
}


class ast::var :
	public ast_node
{
private:
	var_identifier m_identifier;

public:
	var(var_identifier identifier);

	var(const var& other) = default;

	var(var&& other) noexcept;

	var& operator=(const var& other);

	var& operator=(var&& other) noexcept;

	~var() = default;

	[[nodiscard]] const var_identifier& identifier() const;
	
	void accept(ast_node_visitor& visitor) override;
};

