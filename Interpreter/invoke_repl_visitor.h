#pragma once
#include "parser.h"

template <class T>
std::wstring invoke_repl_visitor(const std::wstring& input)
{
	std::wstringstream input_stream;
	input_stream.str(input);
	
	parser parser(input_stream);
    const auto result = parser.parse_repl();

    T visitor;
    visitor.visit(*result);

    return visitor.get_string();
}
