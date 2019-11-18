#pragma once
#include <string>
#include <utility>
#include "token.h"
#include <optional>

class lexer
{
	// Constant
	const std::wstring input;

	// State variables
	wchar_t currentChar;
	size_t pos;
	
	[[nodiscard]] bool is_at_end() const;
	void advance();
	bool skip_whitespace();
	
	token read_digit();
	token read_operator();

public:
	explicit lexer(std::wstring input)
		: input(std::move(input))
	{
		this->pos = 0;
		this->currentChar = !this->input.empty() ? this->input[this->pos] : 0;
	}
	
	/**
	 * Reads the next token from the string
	 */
	token get_next_token();
};

