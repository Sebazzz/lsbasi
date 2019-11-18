#pragma once
#include <string>
#include <utility>
#include "Token.h"
#include <optional>

class Lexer
{
	// Constant
	const std::wstring input;

	// State variables
	wchar_t currentChar;
	size_t pos;
	
	[[nodiscard]] bool is_at_end() const;
	void advance();
	bool skip_whitespace();
	
	Token read_digit();
	Token read_operator();

public:
	explicit Lexer(std::wstring input)
		: input(std::move(input))
	{
		this->pos = 0;
		this->currentChar = this->input.size() > 0 ? this->input[this->pos] : 0;
	}
	
	/**
	 * Reads the next token from the string
	 */
	Token get_next_token();
};

