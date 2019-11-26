#pragma once
#include "ast_node.h"
#include "var.h"

namespace ast
{
	enum class var_type {
		integer,
		real
	};

	class var_decl;
}


class ast::var_decl :
	public ast_node
{
private:
	var_identifier m_identifier;
	var_type m_type;

public:
	var_decl(var_identifier identifier, var_type type);

	~var_decl() = default;

	var_decl(const var_decl& other) = default;

	var_decl(var_decl&& other) noexcept;

	var_decl& operator=(const var_decl& other);

	var_decl& operator=(var_decl&& other) noexcept;

	[[nodiscard]] const var_identifier& identifier() const;
	[[nodiscard]] var_type type() const;
	
	void accept(ast_node_visitor& visitor) override;
};

