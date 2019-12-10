#include "pch.h"
#include "builtin_type_impl.h"
#include "builtin_type_symbol.h"
#include "builtin_type_traits.h"

builtin_type_symbol::builtin_type_symbol(ast::builtin_type type):
	type_symbol(var_type_to_string(type)), m_type(type)
{
	switch (type)
	{
	case ast::builtin_type::boolean:
		this->m_type_impl = std::make_unique<builtin_boolean_impl>(this);
		break;
	case ast::builtin_type::integer:
		this->m_type_impl = std::make_unique<builtin_integer_impl>(this);
		break;
	case ast::builtin_type::real:
		this->m_type_impl = std::make_unique<builtin_real_impl>(this);
		break;
	case ast::builtin_type::string:
		this->m_type_impl = std::make_unique<builtin_string_impl>(this);
		break;
	default:
		throw runtime_type_error("Unsupported type", {static_cast<int>(type), static_cast<int>(type)});
	}
}

ast::builtin_type builtin_type_symbol::type() const
{
	return this->m_type;
}

symbol_identifier builtin_type_symbol::var_type_to_string(ast::builtin_type type)
{
	switch (type)
	{
	case ast::builtin_type::boolean:
		return builtin_type_traits<ast::builtin_type::boolean>::type_name;
	case ast::builtin_type::integer:
		return builtin_type_traits<ast::builtin_type::integer>::type_name;
	case ast::builtin_type::real:
		return builtin_type_traits<ast::builtin_type::real>::type_name;
	case ast::builtin_type::string:
		return builtin_type_traits<ast::builtin_type::string>::type_name;
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
	if (type_spec == ast::builtin_type::boolean)
	{
		static auto boolean_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::boolean);
		return boolean_type;
	}
	
	if (type_spec == ast::builtin_type::integer)
	{
		static auto integer_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::integer);
		return integer_type;
	}

	if (type_spec == ast::builtin_type::real)
	{
		static auto real_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::real);
		return real_type;
	}

	if (type_spec == ast::builtin_type::string)
	{
		static auto string_type = make_symbol_ptr<builtin_type_symbol>(ast::builtin_type::string);
		return string_type;
	}

	throw std::logic_error("Unsupported built-in type as argument");
}
