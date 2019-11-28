#include "pch.h"
#include "symbol_table.h"

void symbol_table::set_from_parent(const ast::var_identifier& identifier, const symbol_info& var_info)
{
	symbol_info v = {var_info.value, var_info.type, true};
	this->m_variables.insert_or_assign(identifier, v);
}

double symbol_table::get(const ast::var_identifier& identifier)
{
	const auto var_val = this->m_variables.find(identifier);

	// Check if it exists
	if (var_val == this->m_variables.end())
	{
		throw interpret_except("Unknown variable in this scope", identifier);
	}

	return var_val->second.value;
}

void symbol_table::declare(const ast::var_identifier& identifier, ast::var_type type)
{
	symbol_info v { 0, type, false };
	const auto success = this->m_variables.try_emplace(identifier, v);

	if (!success.second)
	{
		throw interpret_except("Duplicate declaration", identifier);
	}
}

void symbol_table::set(const ast::var_identifier& identifier, double value)
{
	try
	{
		this->m_variables.at(identifier).value = value;
	} catch (std::exception&)
	{
		throw interpret_except("Undeclared variable", identifier);
	}
}

std::unique_ptr<symbol_table> symbol_table::create_from_parent_scope(const symbol_table* parent)
{
	std::unique_ptr<symbol_table> var_reg(new symbol_table());

	for (auto && pair : parent->m_variables) {	
		const auto& var_info = pair.second;
		var_reg->set_from_parent(pair.first, var_info);
	}
	
	return var_reg;
}

void symbol_table::copy_to_parent()
{
	if (this->m_previous == nullptr)
	{
		return;
	}
	
	for (auto && pair : this->m_variables)
	{
		auto&& parent_var = this->m_previous->m_variables.find(pair.first);
		parent_var->second.value = pair.second.value;
	}
}