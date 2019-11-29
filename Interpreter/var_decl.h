#pragma once
#include "ast_common.h"
#include "type.h"

namespace ast
{
	using type_ptr = ast_node_ptr<type>;
	
	class var_decl;
}

class ast::var_decl :
	public ast_node
{
private:
	var_identifier m_identifier;
	type_ptr m_type;

public:
	var_decl(var_identifier identifier, type_ptr type);

	~var_decl() = default;

	var_decl(const var_decl& other) = default;

	var_decl(var_decl&& other) noexcept;

	var_decl& operator=(const var_decl& other);

	var_decl& operator=(var_decl&& other) noexcept;

	[[nodiscard]] const var_identifier& identifier() const;
	[[nodiscard]] type_ptr type() const;
	
	void accept(ast_node_visitor& visitor) override;
};

