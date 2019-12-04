#include "pch.h"
#include "builtin_type_symbol.h"
#include "symbol_table.h"

symbol_table::symbol_table(): m_scope_name(L"RUNTIME")
{
	// Build up our default, builtin, symbols
	this->m_variables.try_emplace(
		builtin_type_symbol::var_type_to_string(ast::builtin_type::integer), 
		builtin_type_symbol::get_for_builtin_type(ast::builtin_type::integer));
	
	this->m_variables.try_emplace(
		builtin_type_symbol::var_type_to_string(ast::builtin_type::real), 
		builtin_type_symbol::get_for_builtin_type(ast::builtin_type::real));

	this->m_variables.try_emplace(
		builtin_type_symbol::var_type_to_string(ast::builtin_type::string), 
		builtin_type_symbol::get_for_builtin_type(ast::builtin_type::string));
}

symbol_table::symbol_table(std::wstring scope_name, symbol_table* parent): m_parent(parent), m_scope_name(std::move(scope_name))
{
	if (this->m_parent == nullptr)
	{
		static symbol_table runtime_symbol_table;
		this->m_parent = &runtime_symbol_table;
	}
}

symbol_table::symbol_table_iterator::symbol_table_iterator(
	std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator iterator,
	const std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator end):
	iterator(std::move(iterator)), end(end)
{
}

symbol_ptr symbol_table::symbol_table_iterator::operator->() const
{
	return this->iterator->second;
}

symbol_ptr symbol_table::symbol_table_iterator::get() const
{
	return this->iterator->second;
}

bool symbol_table::symbol_table_iterator::is_at_end() const
{
	return this->iterator == this->end;
}

void symbol_table::symbol_table_iterator::next()
{
	++this->iterator;
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
				if (this->m_parent->m_scope_name == L"RUNTIME")
				{
					throw semantic_except(L"Attempt to reference symbol with name '" + identifier + L"' which does not exist in this scope: " + this->m_scope_name, {-1,-1});
				}
				
				throw semantic_except(L"Attempt to reference symbol with name '" + identifier + L"' which does not exist in this scope: " + this->m_scope_name + L" or any parent scope", {-1,-1});
			}
		}
		
		throw semantic_except(L"Attempt to reference symbol with name '" + identifier + L"' which does not exist in this scope: " + this->m_scope_name, {-1,-1});
	}
	
	return symbol_it->second;
}

symbol_type_ptr<type_symbol> symbol_table::get(const ast::builtin_type type_spec)
{
	switch (type_spec)
	{
		case ast::builtin_type::integer:
			return this->get<type_symbol>(L"INTEGER");
		case ast::builtin_type::real:
			return this->get<type_symbol>(L"REAL");
		case ast::builtin_type::string: 
			return this->get<type_symbol>(L"STRING");
		default:
			throw internal_interpret_except("Unsupported internal type");
	}
}

void symbol_table::declare(const symbol_ptr& symbol, line_info position)
{
	if (this->m_variables.find(symbol->identifier()) != this->m_variables.end())
	{
		throw semantic_except(L"Attempt to declare duplicate symbol '" + symbol->to_string() + L"' which already exists in this scope: " + this->m_scope_name, position);
	}

	this->m_variables.insert_or_assign(symbol->identifier(), symbol);
}

symbol_table::symbol_table_iterator symbol_table::iterator() const
{
	return symbol_table_iterator(this->m_variables.begin(), this->m_variables.end());
}
