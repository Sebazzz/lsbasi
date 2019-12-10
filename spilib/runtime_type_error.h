#pragma once
#include "interpret_except.h"

/**
 * Interpretation error which occurs at the semantic analysis phase in which the symbol table is build
 */
class runtime_type_error :
	public exec_error
{
public:
	runtime_type_error(const std::string& message, const ::line_info& line_info);

	runtime_type_error(const std::wstring& message, const ::line_info& line_info);

	explicit runtime_type_error(const interpret_except& other);

	explicit runtime_type_error(interpret_except&& other);
};

