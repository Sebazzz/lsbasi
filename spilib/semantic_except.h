#pragma once
#include "interpret_except.h"

/**
 * Interpretation error which occurs at the semantic analysis phase in which the symbol table is build
 */
class semantic_except :
	public interpret_except
{
public:
	semantic_except(const std::string& message, const ::line_info& line_info);

	semantic_except(const std::wstring& message, const ::line_info& line_info);

	explicit semantic_except(const interpret_except& other);

	explicit semantic_except(interpret_except&& other);
};

