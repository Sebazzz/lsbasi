#pragma once
#include <string>
#include <utility>
#include <memory>
#include "Token.h"
#include <vector>
#include "Lexer.h"

class Interpreter
{
private:
	std::vector<Token> tokens;
	Lexer lexer;
	
	void ensure_tokenized();

	/**
	 * Ensures "-" and following digits are considered negative integers if not prepended by an operator
	 */
	void compact_negative_integers();
	
	void do_tokenize();

	[[nodiscard]] int parse_token_as_integer(const std::vector<Token>::value_type& token) const;

public:
	explicit Interpreter(std::wstring input)
		: lexer(std::move(input))
	{
		this->tokens.reserve(3);
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

