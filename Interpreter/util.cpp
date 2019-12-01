#include "pch.h"
#include "util.h"

std::wstring string_to_wstring(const std::string& input)
{
	// This is a very lossy unicode conversion, but we have to deal with the fact that C++ exception don't use unicode
	const size_t BUFFER_SIZE = 1024;
	wchar_t buffer[BUFFER_SIZE] = {0};

	size_t conv_chars;
	mbstowcs_s(&conv_chars, buffer, input.c_str(), BUFFER_SIZE);

	return std::wstring(buffer, conv_chars);
}

std::string wstring_to_string(const std::wstring& input)
{
	// This is a very lossy unicode conversion, but we have to deal with the fact that C++ exception don't use unicode
	const size_t BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE] = {0};

	size_t conv_chars;
	wcstombs_s(&conv_chars, buffer, input.c_str(), BUFFER_SIZE);

	return std::string(buffer, conv_chars);
}
