#include "pch.h"
#include "memory_table.h"
#include "routine_symbol.h"
#include "symbol_table.h"

memory_table::memory_table(memory_table* parent) : m_parent(parent)
{
}

void memory_table::init_from_symbol_table(symbol_table& symbol_table)
{
	auto iterator = symbol_table.iterator();

	while (!iterator.is_at_end())
	{
		// When copying contents, we only store contents of variables of course.
		// dynamic_cast is used to filter on that
		auto symbol_ptr = iterator.get();
		symbol* symbol = symbol_ptr.get();
		variable_symbol* var_symbol = dynamic_cast<variable_symbol*>(symbol);

		if (var_symbol != nullptr)
		{
			this->m_variables.try_emplace(symbol_ptr, memory_contents { 0 });
		}
		
		iterator.next();
	}

	// Set return value place
	const auto associated_routine = symbol_table.associated_routine();
	if (associated_routine)
	{
		const auto symbol_ptr = std::static_pointer_cast<symbol, routine_symbol>(associated_routine);
		this->m_variables.try_emplace(symbol_ptr, memory_contents { 0 });
	}
}

expression_value memory_table::get(const symbol_ptr& symbol)
{
	const auto it = this->m_variables.find(symbol);

	if (it == this->m_variables.end())
	{
		// Lookup in parent scope if possible
		if (this->m_parent != nullptr)
		{
			return this->m_parent->get(symbol);
		}

		throw internal_interpret_except(L"Cannot find symbol while getting value from memory: " + symbol->to_string());
	}

	return it->second.value;
}

void memory_table::set(const symbol_ptr& symbol, expression_value value)
{
	const auto it = this->m_variables.find(symbol);

	if (it == this->m_variables.end())
	{
		// Lookup in parent scope if possible
		if (this->m_parent != nullptr)
		{
			this->m_parent->set(symbol, value);
			
			return;
		}

		throw internal_interpret_except(L"Cannot find symbol while setting value in memory: " + symbol->to_string());
	}

	it->second.value = value;
}

memory_table* memory_table::create_from_parent_scope(memory_table* parent)
{
	return new memory_table(parent);
}
