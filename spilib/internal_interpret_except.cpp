#include "pch.h"
#include "internal_interpret_except.h"

internal_interpret_except::internal_interpret_except(const std::string& message, const ::line_info& line_info): interpret_except("INTERNAL INTERPRETER ERROR: " + message, line_info)
{
}

internal_interpret_except::internal_interpret_except(const std::wstring& message, const ::line_info& line_info): interpret_except(L"INTERNAL INTERPRETER ERROR: " + message, line_info)
{
}

internal_interpret_except::internal_interpret_except(const interpret_except& other): interpret_except(other)
{
}

internal_interpret_except::internal_interpret_except(interpret_except&& other): interpret_except(other)
{
}
