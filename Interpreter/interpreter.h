#pragma once
#include <string>
#include <utility>
#include <memory>
#include "token.h"
#include <vector>
#include "lexer.h"

// Grammar:
//
// expression: term ((PLUS|MIN)term)*
// term:       factor ((MUL|DIV) factor)*
// factor:     (integer | group)
// group:      "(" expression ")"

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
	void do_tokenize();

	int handle_integer(std::vector<token>::iterator& token) const;
	double handle_factor(std::vector<token>::iterator& it) const;
	double handle_group(std::vector<token>::iterator& it) const;
	double handle_term(std::vector<token>::iterator& it) const;
	double handle_expr(std::vector<token>::iterator& it) const;

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

