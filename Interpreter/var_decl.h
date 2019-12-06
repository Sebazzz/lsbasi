#pragma once
#include "ast_common.h"
#include "type.h"

namespace ast
{
	using type_ptr = ast_node_ptr<type>;
	
	class var_decl;
}

class variable_symbol;
class symbol_table_builder;

class ast::var_decl :
	public ast_node
{
private:
	var_identifier m_identifier;
	type_ptr m_type;
	
	symbol_type_ptr<variable_symbol> m_variable_symbol;

public:
	var_decl(var_identifier identifier, type_ptr type, token token);

	~var_decl() = default;

	var_decl(const var_decl& other) = default;

	var_decl(var_decl&& other) noexcept;

	var_decl& operator=(const var_decl& other);

	var_decl& operator=(var_decl&& other) noexcept;

	[[nodiscard]] const var_identifier& identifier() const;
	[[nodiscard]] type_ptr type() const;
	[[nodiscard]] symbol_type_ptr<variable_symbol> variable_symbol() const;
	
	void accept(ast_node_visitor& visitor) override;
	
	friend symbol_table_builder;
};

