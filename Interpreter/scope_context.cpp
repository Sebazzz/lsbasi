#include "pch.h"
#include "scope_context.h"
#include "memory_table.h"

scope_context& scope_manager::current_context()
{
	if (this->m_scope.empty())
	{
		throw interpret_except("Expected non-empty stack");
	}

	return this->m_scope.top();
}

scope_context& scope_manager::push()
{
	if (!this->m_scope.empty())
	{
		const auto& top_scope = this->m_scope.top();
		this->m_scope.push(scope_context { memory_table::create_from_parent_scope(top_scope.symbols.get()) });
	} else
	{
		this->m_scope.push(scope_context { std::make_unique<memory_table>() });
	}
	
	return this->m_scope.top();
}

void scope_manager::pop()
{
	if (this->m_scope.empty())
	{
		throw interpret_except("Expected non-empty stack");
	}

	this->m_scope.top().symbols->copy_to_parent();
	this->m_scope.pop();
}
