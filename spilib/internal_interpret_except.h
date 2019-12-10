#pragma once
#include "interpret_except.h"

/**
 * Interpretation error which occurs when the interpreter comes into a unexpected state. This is a programming error.
 * I'd use an assert in this case instead, but the guidance states that it is recommended to throw an exception instead
 */
class internal_interpret_except :
	public interpret_except
{
public:
	internal_interpret_except(const std::string& message, const ::line_info& line_info = {-1,-1});

	internal_interpret_except(const std::wstring& message, const ::line_info& line_info = {-1,-1});

	explicit internal_interpret_except(const interpret_except& other);

	explicit internal_interpret_except(interpret_except&& other);
};

