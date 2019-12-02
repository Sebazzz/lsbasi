#include "pch.h"
#include "builtin_type_impl.h"
#include "builtin_type_symbol.h"

builtin_type_symbol::builtin_type_symbol(ast::builtin_type type):
	type_symbol(var_type_to_string(type)),
	m_type(type),
	m_type_impl(std::make_unique<builtin_type_impl>(this))
{
}

ast::builtin_type builtin_type_symbol::type() const
{
	return this->m_type;
}

symbol_identifier builtin_type_symbol::var_type_to_string(ast::builtin_type type)
{
	switch (type)
	{
	case ast::builtin_type::integer:
		return L"INTEGER";
	case ast::builtin_type::real:
		return L"REAL";
	case ast::builtin_type::string:
		return L"STRING";
	default:
		throw std::logic_error("Unknown built-in variable type");
		;
	}
}

std::wstring builtin_type_symbol::to_string() const
{
	return L"built-in " + this->identifier();
}

const ::type_impl& builtin_type_symbol::type_impl()
{
	return *this->m_type_impl;
}

symbol_type_ptr<type_symbol> builtin_type_symbol::get_for_builtin_type(ast::builtin_type type_spec)
{
	if (type_spec == ast::builtin_type::integer)
	{
		static symbol_ptr integer_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::integer);
		return std::static_pointer_cast<type_symbol, symbol>(integer_type);
	}

	if (type_spec == ast::builtin_type::real)
	{
		static symbol_ptr real_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::real);
		return std::static_pointer_cast<type_symbol, symbol>(real_type);
	}

	if (type_spec == ast::builtin_type::string)
	{
		static symbol_ptr string_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::string);
		return std::static_pointer_cast<type_symbol, symbol>(string_type);
	}

	throw std::logic_error("Unsupported built-in type as argument");
}
