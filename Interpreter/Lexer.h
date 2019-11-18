#pragma once
#include <string>
#include <utility>
#include "Token.h"

class Lexer
{
	std::wstring input;
	size_t pos;

public:
	explicit Lexer(std::wstring input)
		: input(std::move(input))
	{
		this->pos = 0;
	}
	
	/**
	 * Reads the next token from the string
	 */
	Token get_next_token();
};

