#pragma once
#include <string>
#include <utility>
#include "token.h"
#include <optional>

class lexer
{
	/**
	 * Returned by peek method to indicate that we reached the end
	 */
	const static wchar_t NO_NEXT_CHAR = 0;
	
	// Constant
	const std::wstring input;

	// State variables
	wchar_t currentChar;
	size_t pos;

	[[nodiscard]] bool is_at_end() const;
	void advance();

	/**
	 * Peeks the next character or returns NO_NEXT_CHAR on failure
	 */
	[[nodiscard]] wchar_t peek() const; // Considered returning a std::optional instead, but a constant works just as well

	bool skip_whitespace();
	bool skip_comment();

	token read_digit();
	token read_operator();
	token read_identifier_or_keyword();

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

