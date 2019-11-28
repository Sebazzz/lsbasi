#pragma once
#include "symbol_table.h"

struct scope_context
{
	std::unique_ptr<symbol_table> symbols;
};

class scope_manager
{
private:
	std::stack<scope_context> m_scope;

public:
	/**
	 * Gets the current context. Throws if not available.
	 */
	scope_context& current_context();

	/**
	 * 
	 */
	scope_context& push();

	/**
	 * Reverts to the previous scope
	 */
	void pop();
};
