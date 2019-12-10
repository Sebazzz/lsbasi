#pragma once
#include "pch.h"
#include "symbol.h"

using ast::expression_value;

class symbol_table;
class memory_table
{
private:
	struct memory_contents
	{
		expression_value value;

		memory_contents() = default;
	};
	
	std::map<symbol_ptr, memory_contents> m_variables;

	/**
	 * Contains a pointer to the parent scope. I believe raw pointer usage is justified because
	 * we always either have a parent scope (thus memory) or we don't but the parent scope can never be dangling.
	 */
	memory_table* m_parent = nullptr;

	explicit memory_table(memory_table* parent);
	

public:
	memory_table() = default;
	
	expression_value get(const symbol_ptr& symbol);

	void set(const symbol_ptr& symbol, expression_value value);

	static memory_table* create_from_parent_scope(memory_table* parent);

	/**
	 * Ensures all the known symbols are present
	 */
	void init_from_symbol_table(symbol_table& symbol_table);
};