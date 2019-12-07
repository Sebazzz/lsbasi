#pragma once
#include "ast_common.h"
#include "symbol_types.h"

namespace ast
{
	class assignment_target;
}

class type_symbol;
class variable_symbol;
class routine_symbol;
class symbol_table_builder;

class ast::assignment_target : public ast_node
{
private:
	var_identifier m_identifier;

	symbol_type_ptr<variable_symbol> m_variable_symbol;
	symbol_type_ptr<routine_symbol> m_function_symbol;

public:
	assignment_target(var_identifier identifier, token token);

	assignment_target(const assignment_target& other) = default;

	assignment_target(assignment_target&& other) noexcept;

	assignment_target& operator=(const assignment_target& other);

	assignment_target& operator=(assignment_target&& other) noexcept;

	~assignment_target() = default;

	[[nodiscard]] const var_identifier& identifier() const;
	[[nodiscard]] symbol_type_ptr<type_symbol> type_symbol() const;
	[[nodiscard]] symbol_type_ptr<symbol> assignment_symbol() const;
	
	void accept(ast_node_visitor& visitor) override;

	friend symbol_table_builder;
};

