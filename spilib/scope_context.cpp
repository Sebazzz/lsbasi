#include "pch.h"
#include "scope_context.h"
#include "program.h"
#include "memory_table.h"
#include "symbol_table_builder.h"


scope_context::scope_context(symbol_table& symbol_table): memory(new memory_table()), symbols(symbol_table)
{
	this->memory->init_from_symbol_table(symbol_table);
}

scope_context::scope_context(symbol_table& symbol_table, memory_table* memory_table): memory(memory_table), symbols(symbol_table)
{
	this->memory->init_from_symbol_table(symbol_table);
}

scope_context::scope_context(const scope_context& other): memory(new memory_table(*other.memory)), symbols(other.symbols)
{
}

scope_context::scope_context(scope_context&& other) noexcept: memory(std::move(other.memory)), symbols(other.symbols)
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


scope_context& scope_manager::push(symbol_table& symbol_table)
{
	if (!this->m_scope.empty())
	{
		const auto& top_scope = this->m_scope.top();
		this->m_scope.push(scope_context(symbol_table, memory_table::create_from_parent_scope(top_scope.memory.get())));
	} else
	{
		this->m_scope.push(scope_context(symbol_table, memory_table::create_from_parent_scope(this->m_global_scope->memory.get())));
	}
	
	return this->m_scope.top();
}

scope_context& scope_manager::apply_prepared_scope(scope_context& new_scope)
{
	this->m_scope.push(std::move(new_scope));

	return this->m_scope.top();
}

scope_context scope_manager::prepare_new_scope(symbol_table& symbol_table) const
{
	return scope_context(symbol_table, memory_table::create_from_parent_scope(this->m_global_scope->memory.get()));
}

void scope_manager::pop()
{
	if (this->m_scope.empty())
	{
		throw exec_error("Expected non-empty stack", {-1,-1});
	}

	this->m_scope.pop();
}

void scope_manager::create_global_scope(ast::program& program)
{
	symbol_table_builder symbol_visitor;
	symbol_visitor.visit(program);
	
	this->m_global_scope = std::make_shared<scope_context>(program.symbol_table());
}

std::shared_ptr<scope_context> scope_manager::global_scope() const
{
	if (!this->m_scope.empty())
	{
		throw std::logic_error("This method is for unit testing purposes and should be called in the top scope.");
	}

	if (!this->m_global_scope)
	{
		throw std::logic_error("This method is for unit testing purposes - the global scope has not been created yet");
	}

	return this->m_global_scope;
}
