#include "pch.h"
#include "symbol_table.h"

symbol_table::symbol_table(std::wstring scope_name, symbol_table* parent): m_parent(parent), m_scope_name(std::move(scope_name))
{
	// Build up our default, builtin, symbols
	static symbol_ptr integer_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::integer);
	static symbol_ptr real_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::real);
	
	this->m_variables.try_emplace(builtin_type_symbol::var_type_to_string(ast::builtin_type::integer), integer_type);
	this->m_variables.try_emplace(builtin_type_symbol::var_type_to_string(ast::builtin_type::real), real_type);
}

symbol_ptr symbol_table::get(const ast::var_identifier& identifier)
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
		}
		
		throw interpret_except(L"Attempt to reference symbol with name '" + identifier + L"' which does not exist in this scope: " + this->m_scope_name);
	}
	
	return symbol_it->second;
}

void symbol_table::declare(const symbol_ptr& symbol)
{
	if (this->m_variables.find(symbol->identifier()) != this->m_variables.end())
	{
		throw interpret_except(L"Attempt to declare duplicate symbol '" + symbol->to_string() + L"' which already exists in this scope: " + this->m_scope_name);
	}

	this->m_variables.insert_or_assign(symbol->identifier(), symbol);
}

symbol_table::symbol_table_iterator symbol_table::iterator() const
{
	return symbol_table_iterator(this->m_variables.begin(), this->m_variables.end());
}
