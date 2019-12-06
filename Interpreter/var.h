#pragma once
#include "ast_common.h"
#include "symbol_types.h"

namespace ast
{
	class var;
}

class variable_symbol;
class symbol_table_builder;

class ast::var : public ast_node
{
private:
	var_identifier m_identifier;

	symbol_type_ptr<variable_symbol> m_variable_symbol;

public:
	var(var_identifier identifier, token token);

	var(const var& other) = default;

	var(var&& other) noexcept;

	var& operator=(const var& other);

	var& operator=(var&& other) noexcept;

	~var() = default;

	[[nodiscard]] const var_identifier& identifier() const;
	[[nodiscard]] symbol_type_ptr<variable_symbol> variable_symbol() const;
	
	void accept(ast_node_visitor& visitor) override;

	friend symbol_table_builder;
};

