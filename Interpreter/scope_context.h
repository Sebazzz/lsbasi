#pragma once
#include <utility>
#include "pch.h"
#include "symbol_table.h"

namespace ast {
	class program;
}

class memory_table;

struct scope_context
{
	std::unique_ptr<memory_table> memory;
	symbol_table& symbols;

	scope_context() = delete;
	~scope_context() = default;

	explicit scope_context(symbol_table& symbol_table,
	                       const std::unique_ptr<memory_table>& memory_table);

	scope_context(const scope_context& other);

	scope_context(scope_context&& other) noexcept;

	scope_context& operator=(const scope_context& other);
};

class scope_manager
{
private:
	std::stack<scope_context> m_scope;

	/**
	 * Represents the global scope. Not available until create_global_scope is called.
	 */
	std::shared_ptr<scope_context> m_global_scope;

public:
	/**
	 * Gets the current context - at the top level this is the global scope
	 */
	scope_context& current_context();

	/**
	 * Enters a new scope with the current scope as parent
	 */
	scope_context& push(symbol_table& symbol_table);

	/**
	 * Enters a new scope (procedure)
	 */
	scope_context& new_scope(symbol_table& symbol_table);

	/**
	 * Reverts to the previous scope.
	 */
	void pop();

	/**
	 * Internal method to create the initial global scope
	 */
	void create_global_scope(ast::program& program);

	/**
	 * Gets the global scope. This is normally not necessary, but useful for unit testing purposes.
	 */
	[[nodiscard]] std::shared_ptr<scope_context> global_scope() const;
};
