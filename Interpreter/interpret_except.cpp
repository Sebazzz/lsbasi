#include "pch.h"

interpret_except::interpret_except(const std::string& message)
{
	m_message = message;
}

interpret_except::interpret_except(const std::wstring& message)
{
	m_message = wstring_to_string(message);
}

interpret_except::interpret_except(exception const& _Other): exception(_Other)
{
}

char const* interpret_except::what() const
{
	return this->m_message.c_str();
}
