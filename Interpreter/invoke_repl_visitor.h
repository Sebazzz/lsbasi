#pragma once
#include <string>
#include "parser.h"

template <class T>
std::wstring invoke_repl_visitor(std::wstring& input)
{
	parser parser(std::move(input));
    const auto result = parser.parse();

    T visitor;
    visitor.visit(*result);

    return visitor.get_string();
}
