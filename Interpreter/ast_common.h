#pragma once
// ReSharper disable CppUnusedIncludeDirective - utility header
#include "ast_node.h"
#include "builtin_type_defs.h"

// ReSharper restore CppUnusedIncludeDirective

namespace ast {
	// Common typedefs
	using identifier_type = std::wstring;
	using var_identifier = identifier_type;
	using type_identifier = identifier_type;
	using procedure_identifier = std::wstring;

	enum class builtin_type {
		integer,
		real
	};

	/**
	 * Represents the result of an expression
	 */
	union expression_value
	{
		builtin_integer int_val;
		builtin_real real_val;

		expression_value(builtin_integer value) : int_val(value){}
		expression_value(builtin_real value) : real_val(value){}
	};

	// Note about data type size of expression_value:
	// a double is 64-bit so it is at max 64-bits. 
	static_assert(sizeof(expression_value) == sizeof(double), "Expect data size to be at most 64-bit (8 bytes)");
	

	// Fwd declarations
	class compound;
	class var_decl;
	class procedure;

	// Common pointer types
	using compound_ptr = ast_node_ptr<compound>;
	using procedure_decl_ptr = ast_node_ptr<procedure>;
	using var_decl_ptr = ast_node_ptr<var_decl>;
}
