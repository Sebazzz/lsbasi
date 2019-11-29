#include "pch.h"
#include "symbol_table.h"

symbol_table::symbol_table(std::wstring scope_name, symbol_table* parent): m_parent(parent), m_scope_name(std::move(scope_name))
{
}

const symbol& symbol_table::get(const ast::var_identifier& identifier)
{
	const auto& symbol_it = this->m_variables.find(identifier);

	if (symbol_it == this->m_variables.end())
	{
		if (this->m_parent != nullptr)
		{
			try
			{
				return this->m_parent->get(identifier);
			} catch (interpret_except&)
			{
				throw interpret_except(L"Attempt to reference symbol with name '" + identifier + L"' which does not exist in this scope: " + this->m_scope_name + L" or any parent scope");
			}
		} else
		{
			throw interpret_except(L"Attempt to reference symbol with name '" + identifier + L"' which does not exist in this scope: " + this->m_scope_name);
		}
	}

	return *symbol_it->second;
}

void symbol_table::declare(const symbol_ptr& symbol)
{
	if (this->m_variables.find(symbol->identifier()) != this->m_variables.end())
	{
		throw interpret_except(L"Attempt to declare duplicate symbol '" + symbol->to_string() + L"' which already exists in this scope: " + this->m_scope_name);
	}

	this->m_variables.insert_or_assign(symbol->identifier(), symbol);
}
