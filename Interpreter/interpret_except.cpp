#include "interpret_except.h"

interpret_except::interpret_except(const std::string& message)
{
	m_message = message;
}

interpret_except::interpret_except(const std::string& message, const std::string& actual)
{
	m_message = message + " - " + actual;
}

interpret_except::interpret_except(const std::string& message, const std::wstring& actual)
{
	// This is a very lossy unicode conversion, but we have to deal with the fact that C++ exception don't use unicode
	const size_t BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE] = {0};

	size_t conv_chars;
	wcstombs_s(&conv_chars, buffer, actual.c_str(), BUFFER_SIZE);
	
	m_message = message + " - " + buffer;
}

interpret_except::interpret_except(exception const& _Other): exception(_Other)
{
}

char const* interpret_except::what() const
{
	return this->m_message.c_str();
}
