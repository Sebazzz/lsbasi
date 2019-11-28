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

	// Fwd declarations
	class compound;
	class var_decl;
	class procedure;

	// Common pointer types
	using compound_ptr = ast_node_ptr<compound>;
	using procedure_decl_ptr = ast_node_ptr<procedure>;
	using var_decl_ptr = ast_node_ptr<var_decl>;
}
