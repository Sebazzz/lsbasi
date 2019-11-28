#pragma once
#include "pch.h"

class memory_table;

struct scope_context
{
	std::unique_ptr<memory_table> memory;

	scope_context() = default;
	~scope_context() = default;

	explicit scope_context(const std::unique_ptr<memory_table>& memory_table): memory(memory_table.get())
	{
	}
	
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
	std::unique_ptr<scope_context> m_global_scope;

public:
	/**
	 * Gets the current context - at the top level this is the global scope
	 */
	scope_context& current_context();

	/**
	 * Enters a new scope
	 */
	scope_context& push();

	/**
	 * Reverts to the previous scope.
	 */
	void pop();

	/**
	 * Internal method to create the initial global scope
	 */
	void create_global_scope();

	/**
	 * Gets the global scope. This is normally not necessary, but useful for unit testing purposes.
	 */
	const scope_context& global_scope() const;
};
