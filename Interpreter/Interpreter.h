#pragma once
#include <string>
#include <utility>
#include <memory>
#include "Token.h"
#include <vector>
#include <optional>

class Interpreter
{
private:
	std::wstring input;
	std::vector<Token> tokens;

	std::optional<Token> currentToken;
	size_t pos;

	void ensure_tokenized();

	/**
	 * Ensures "-" and following digits are considered negative integers if not prepended by an operator
	 */
	void compact_negative_integers();
	
	void do_tokenize();

	/**
	 * Reads the next token from the string
	 */
	Token get_next_token();

	[[nodiscard]] int parse_token_as_integer(const std::vector<Token>::value_type& token) const;

public:
	explicit Interpreter(std::wstring input)
		: input(std::move(input))
	{
		this->tokens.reserve(3);
		this->pos = 0;
	}

	/**
	 * Tokenizes the string and return string representation of the tokens
	 */
	std::wstring tokenize();

	/**
	 * Interprets (executes) the string
	 */
	std::wstring interpret();
};

