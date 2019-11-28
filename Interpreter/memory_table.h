#pragma once
#include "pch.h"

struct memory_contents
{
	ast::symbol_value value;
	ast::var_type type;

	[[nodiscard]] std::wstring to_string() const;
};

class memory_table
{
private:
	struct symbol_info
	{
		memory_contents symbol_contents;
		bool is_from_parent_scope;
	};
	
	std::map<ast::var_identifier, symbol_info, case_insensitive_string_comparer> m_variables;

	void set_from_parent(const ast::var_identifier& identifier, const symbol_info& var_info);

	/**
	 * Contains a pointer to the parent scope. I believe raw pointer usage is justified because
	 * we always either have a previous scope (thus symbols) or we don't but the previous scope can never be dangling.
	 */
	memory_table* m_previous = nullptr;

public:
	memory_contents get(const ast::var_identifier& identifier);

	void declare(const ast::var_identifier& identifier, ast::var_type type);

	void ensure_type(const ast::var_identifier& identifier, ast::var_type type);
	
	void set(const ast::var_identifier& identifier, ast::symbol_value value);

	static std::unique_ptr<memory_table> create_from_parent_scope(const memory_table* parent);
	
	void copy_to_parent();
};