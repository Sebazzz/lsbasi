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

	union num_value
	{
		int int_val;
		double real_val;

		num_value(int value) : int_val(value){}
		num_value(double value) : real_val(value){}
	};
}
