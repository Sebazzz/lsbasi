#include "pch.h"
#include "parse_except.h"

parse_except::parse_except(const std::string& message, const ::line_info& line_info): interpret_except("Syntax error: " + message, line_info)
{
}

parse_except::parse_except(const std::wstring& message, const ::line_info& line_info): interpret_except(L"Syntax error: " +message, line_info)
{
}

parse_except::parse_except(const interpret_except& other): interpret_except(other)
{
}

parse_except::parse_except(interpret_except&& other): interpret_except(other)
{
}
