#pragma once
#include "ast_common.h"
#include "symbol_types.h"

/**
 * Represents type information for a built-in type. This is necessary to properly generalize templates which use
 * built-in types. This base implementation is supposed to cause a compilation failure when a ast::builtin_type has been
 * provided for which no template specialization has been provided.
 */
template<ast::builtin_type T>
struct builtin_type_traits
{
	using builtin_type = void*;
	static constexpr wchar_t type_name[] = L"INVALID";

	static symbol_identifier param_id()
	{
		static_assert(false, L"Invalid template - this relies on specialization");
		return L"?";
	}

	static builtin_type get_from_expression(ast::expression_value& expression_value)
	{
		return &expression_value;
	}
};

template<>
struct builtin_type_traits<ast::builtin_type::string>
{
	using builtin_type = builtin_string_ptr;
	static constexpr wchar_t type_name[] = L"STRING";

	static symbol_identifier param_id()
	{
		return L"str_arg";
	}

	static builtin_type get_from_expression(ast::expression_value& expression_value)
	{
		return expression_value.string_ptr_val;
	}
};


template<>
struct builtin_type_traits<ast::builtin_type::integer>
{
	using builtin_type = builtin_integer;
	static constexpr wchar_t type_name[] = L"INTEGER";

	static symbol_identifier param_id()
	{
		return L"int_val";
	}

	static builtin_type get_from_expression(ast::expression_value& expression_value)
	{
		return expression_value.int_val;
	}
};


template<>
struct builtin_type_traits<ast::builtin_type::real>
{
	using builtin_type = builtin_real;
	static constexpr wchar_t type_name[] = L"REAL";

	static symbol_identifier param_id()
	{
		return L"real_arg";
	}

	static builtin_type get_from_expression(ast::expression_value& expression_value)
	{
		return expression_value.real_val;
	}
};