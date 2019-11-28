#include "pch.h"
#include "scope_context.h"
#include "memory_table.h"

scope_context::scope_context(const scope_context& other): memory(new memory_table(*other.memory))
{
}

scope_context::scope_context(scope_context&& other) noexcept: memory(std::move(other.memory))
{
}

scope_context& scope_context::operator=(const scope_context& other)
{
	if (this == &other)
		return *this;
	memory = std::make_unique<memory_table>(*other.memory);
	return *this;
}

scope_context& scope_manager::current_context()
{
	// At the top of our program we only have the global scope
	if (this->m_scope.empty())
	{
		if (!this->m_global_scope)
		{
			throw std::logic_error("Interpreter internal error - a global scope should have been defined");
		}

		return *this->m_global_scope;
	}

	return this->m_scope.top();
}

scope_context& scope_manager::push()
{
	if (!this->m_scope.empty())
	{
		const auto& top_scope = this->m_scope.top();
		this->m_scope.push(scope_context(memory_table::create_from_parent_scope(top_scope.memory.get())));
	} else
	{
		this->m_scope.push(scope_context(memory_table::create_from_parent_scope(this->m_global_scope->memory.get())));
	}
	
	return this->m_scope.top();
}

void scope_manager::pop()
{
	if (this->m_scope.empty())
	{
		throw interpret_except("Expected non-empty stack");
	}

	this->m_scope.top().memory->copy_to_parent();
	this->m_scope.pop();
}

void scope_manager::create_global_scope()
{
	this->m_global_scope = std::make_unique<scope_context>();
	this->m_global_scope->memory = std::make_unique<memory_table>();
}

const scope_context& scope_manager::global_scope() const
{
	if (!this->m_scope.empty())
	{
		throw std::logic_error("This method is for unit testing purposes and should be called in the top scope.");
	}

	if (!this->m_global_scope)
	{
		throw std::logic_error("This method is for unit testing purposes - the global scope has not been created yet");
	}

	return *this->m_global_scope;
}
