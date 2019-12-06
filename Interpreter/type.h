#pragma once
#include "ast_common.h"
#include "symbol_types.h"

namespace ast
{
	class type;
}

class type_symbol;
class symbol_table_builder;

class ast::type :
	public ast_node
{
private:
	type_identifier m_identifier;
	symbol_type_ptr<type_symbol> m_type_symbol;

public:
	type(type_identifier identifier, token token);

	type(const type& other) = default;

	type(type&& other) noexcept;

	type& operator=(const type& other);

	type& operator=(type&& other) noexcept;

	~type() = default;

	[[nodiscard]] const type_identifier& identifier() const;
	[[nodiscard]] symbol_type_ptr<type_symbol> type_symbol() const;
	
	void accept(ast_node_visitor& visitor) override;
	
	friend symbol_table_builder;
};

