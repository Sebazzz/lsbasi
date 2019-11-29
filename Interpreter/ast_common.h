#pragma once
// ReSharper disable CppUnusedIncludeDirective - utility header
#include "ast_node.h"
// ReSharper restore CppUnusedIncludeDirective

namespace ast {
	// Common typedefs
	using identifier_type = std::wstring;
	using var_identifier = identifier_type;
	using type_identifier = identifier_type;

	enum class builtin_type {
		integer,
		real
	};

	union expression_value
	{
		int int_val;
		double real_val;

		expression_value(int value) : int_val(value){}
		expression_value(double value) : real_val(value){}
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
