#include "pch.h"
#include "runtime_type_error.h"

runtime_type_error::runtime_type_error(const std::string& message, const ::line_info& line_info): exec_error("Runtime type error: " + message, line_info)
{
}

runtime_type_error::runtime_type_error(const std::wstring& message, const ::line_info& line_info): exec_error(L"Runtime type error: " + message, line_info)
{
}

runtime_type_error::runtime_type_error(const interpret_except& other): exec_error(other)
{
}

runtime_type_error::runtime_type_error(interpret_except&& other): exec_error(other)
{
}
