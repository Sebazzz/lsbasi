#include "pch.h"
#include "semantic_except.h"

semantic_except::semantic_except(const std::string& message, const ::line_info& line_info): interpret_except(
	message, line_info)
{
}

semantic_except::semantic_except(const std::wstring& message, const ::line_info& line_info): interpret_except(
	message, line_info)
{
}

semantic_except::semantic_except(const interpret_except& other): interpret_except(other)
{
}

semantic_except::semantic_except(interpret_except&& other): interpret_except(other)
{
}
