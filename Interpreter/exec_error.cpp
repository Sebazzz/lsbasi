#include "pch.h"
#include "exec_error.h"

exec_error::exec_error(const std::string& message, const ::line_info& line_info): interpret_except(
	message, line_info)
{
}

exec_error::exec_error(const std::wstring& message, const ::line_info& line_info): interpret_except(
	message, line_info)
{
}

exec_error::exec_error(const interpret_except& other): interpret_except(other)
{
}

exec_error::exec_error(interpret_except&& other): interpret_except(other)
{
}
