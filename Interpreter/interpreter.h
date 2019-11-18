#pragma once
#include <string>
#include <utility>
#include <memory>
#include "token.h"
#include <vector>
#include "lexer.h"

// Grammar:
//
// expression: factor ((MUL|DIV|PLUS|MIN)factor)*
// factor:     integer

/**
 *
 * Interpret simple math expressions
 * 
 */
class interpreter
{
private:
	std::vector<token> tokens;
	lexer lexer;
	
	void ensure_tokenized();

	/**
	 * Ensures "-" and following digits are considered negative integers if not prepended by an operator
	 */
	void compact_negative_integers();
	
	void do_tokenize();

	[[nodiscard]] int get_integer(std::vector<token>::iterator& token) const;
	bool handle_operator(double& result, std::vector<token>::iterator& it);

public:
	explicit interpreter(std::wstring input)
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

