#pragma once
#include "interpret_except.h"

/**
 * Interpretation error which occurs at the parsing phase
 */
class parse_except :
	public interpret_except
{
public:
	parse_except(const std::string& message, const ::line_info& line_info);

	parse_except(const std::wstring& message, const ::line_info& line_info);

	explicit parse_except(const interpret_except& other);

	explicit parse_except(interpret_except&& other);
};

