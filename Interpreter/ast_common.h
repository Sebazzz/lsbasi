#pragma once
// ReSharper disable CppUnusedIncludeDirective - utility header
#include "ast_node.h"
// ReSharper restore CppUnusedIncludeDirective

namespace ast
{
	using var_identifier = std::wstring;

	enum class var_type {
		integer,
		real
	};

	union symbol_value
	{
		int int_val;
		double real_val;

		symbol_value(int value) : int_val(value){}
		symbol_value(double value) : real_val(value){}
	};
}
