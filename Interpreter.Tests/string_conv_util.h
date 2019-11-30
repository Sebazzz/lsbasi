#pragma once
#include <string>

/**
 * For the tests it is very practical to use raw C++ strings
 * because the support being multiline natively. Raw strings are char
 * instead of wchar_t which is kind-of inconvenient for the parser which accepts wchar_t only.
 */
inline std::wstring raw_to_wstring(const char* input)
{
	/**
	 * 1024 chars ought to be enough for every test
	 */
	const size_t BUFFER_SIZE = 1024;
	wchar_t buffer[BUFFER_SIZE] = {0};

	size_t conv_chars;
	mbstowcs_s(&conv_chars, buffer, input, BUFFER_SIZE);

	return buffer;
}