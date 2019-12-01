#pragma once
#include "ast_common.h"

namespace ast
{
	class type;
}


class ast::type :
	public ast_node
{
private:
	type_identifier m_identifier;

public:
	type(type_identifier identifier, token token);

	type(const type& other) = default;

	type(type&& other) noexcept;

	type& operator=(const type& other);

	type& operator=(type&& other) noexcept;

	~type() = default;

	[[nodiscard]] const type_identifier& identifier() const;
	
	void accept(ast_node_visitor& visitor) override;
};

