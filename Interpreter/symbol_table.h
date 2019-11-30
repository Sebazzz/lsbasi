#pragma once
#include <utility>
#include "pch.h"
#include "symbol.h"

class symbol_table
{
private:
	std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer> m_variables;

	/**
	 * Contains a pointer to the parent symbol table. I believe raw pointer usage is justified because
	 * we always either have a parent scope (thus memory) or we don't but the previous scope can never be dangling.
	 */
	symbol_table* m_parent = nullptr;

	std::wstring m_scope_name;

public:
	explicit symbol_table(std::wstring scope_name, symbol_table* parent = nullptr);

	class symbol_table_iterator
	{
		friend class symbol_table;

		std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator iterator;
		const std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator end;

		explicit symbol_table_iterator(
			std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator iterator,
			const std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator end);

	public:
		symbol_ptr operator->() const;

		[[nodiscard]] symbol_ptr get() const;

		[[nodiscard]] bool is_at_end() const;

		void next();
	};
	
	/**
	 * Gets a symbol from the current or parent scope. Throws if not found.
	 */
	symbol_ptr get(const ast::var_identifier& identifier);

	/**
	 * Declares a symbol. Throws if already declared.
	 */
	void declare(const symbol_ptr& symbol);

	/**
	 * Gets an iterator at allows iterating over all the symbols
	 */
	[[nodiscard]] symbol_table_iterator iterator() const;
};
