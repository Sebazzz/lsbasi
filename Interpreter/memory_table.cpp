#include "pch.h"
#include "memory_table.h"

memory_table::memory_table(memory_table* parent) : m_parent(parent)
{
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

		throw std::logic_error("Cannot find symbol");
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

		throw std::logic_error("Cannot find symbol");
	}

	it->second.value = value;
}

std::unique_ptr<memory_table> memory_table::create_from_parent_scope(memory_table* parent)
{
	std::unique_ptr<memory_table> var_reg(new memory_table(parent));

	return var_reg;
}
