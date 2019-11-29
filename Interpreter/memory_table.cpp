#include "pch.h"
#include "memory_table.h"

std::wstring memory_contents::to_string() const
{
	std::wstring output;
	switch (this->type)
	{
	case ast::var_type::integer:
		output += L"integer [";
		output += std::to_wstring(this->value.int_val);
		output += L"]";
		break;
	case ast::var_type::real:
		output += L"real - [";
		output += std::to_wstring(this->value.int_val);
		output += L"]";
		break;
	default:
		output = L"unknown";
	}

	return output;
}

void memory_table::set_from_parent(const ast::var_identifier& identifier, const symbol_info& var_info)
{
	symbol_info v = {var_info.symbol_contents, true};
	this->m_variables.insert_or_assign(identifier, v);
}

memory_contents memory_table::get(const ast::var_identifier& identifier)
{
	const auto var_val = this->m_variables.find(identifier);

	// Check if it exists
	if (var_val == this->m_variables.end())
	{
		throw interpret_except("Unknown variable in this scope", identifier);
	}

	return var_val->second.symbol_contents;
}

void memory_table::declare(const ast::var_identifier& identifier, ast::var_type type)
{
	symbol_info v { 0, type, false };
	const auto success = this->m_variables.try_emplace(identifier, v);

	if (!success.second)
	{
		throw interpret_except("Duplicate declaration", identifier);
	}
}

void memory_table::ensure_type(const ast::var_identifier& identifier, ast::var_type type)
{
	const auto var_info = this->m_variables.find(identifier);

	// Check if it exists
	if (var_info == this->m_variables.end())
	{
		throw interpret_except("Unknown variable in this scope", identifier);
	}

	if (var_info->second.symbol_contents.type != type)
	{
		throw interpret_except("Variable is not of expected type", identifier);
	}
}

void memory_table::set(const ast::var_identifier& identifier, ast::expression_value value)
{
	try
	{
		this->m_variables.at(identifier).symbol_contents.value = value;
	} catch (std::exception&)
	{
		throw interpret_except("Undeclared variable", identifier);
	}
}

std::unique_ptr<memory_table> memory_table::create_from_parent_scope(const memory_table* parent)
{
	std::unique_ptr<memory_table> var_reg(new memory_table());

	for (auto && pair : parent->m_variables) {	
		const auto& var_info = pair.second;
		var_reg->set_from_parent(pair.first, var_info);
	}
	
	return var_reg;
}

void memory_table::copy_to_parent()
{
	if (this->m_previous == nullptr)
	{
		return;
	}
	
	for (auto && pair : this->m_variables)
	{
		auto&& parent_var = this->m_previous->m_variables.find(pair.first);
		parent_var->second.symbol_contents = pair.second.symbol_contents;
	}
}