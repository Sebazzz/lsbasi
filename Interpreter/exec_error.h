#pragma once
#include "interpret_except.h"

/**
 * Interpretation error which occurs at the execution phase when the given code is actually executed
 */
class exec_error :
	public interpret_except
{
public:
	exec_error(const std::string& message, const ::line_info& line_info);

	exec_error(const std::wstring& message, const ::line_info& line_info);

	explicit exec_error(const interpret_except& other);

	explicit exec_error(interpret_except&& other);
};

