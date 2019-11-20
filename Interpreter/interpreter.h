#pragma once
#include <string>
#include <utility>
#include <memory>
#include "lexer.h"
#include "ast_node.h"
#include "bin_op.h"
#include "num.h"
#include "parser.h"

/**
 *
 * Interpret simple math expressions
 * 
 */
class interpreter
{
private:
	parser parser;
	
	ast_ptr parsed_ast;

	void ensure_parsed();
	void do_parse();

public:
	explicit interpreter(std::wstring input)
		: parser(std::move(input))
	{
	}

	/**
	 * Tokenizes the string and return string representation of the tokens
	 */
	std::wstring tokenize();

	/**
	 * Stringifies the abstract syntax tree
	 */
	std::wstring stringify_ast();

	/**
	 * Interprets (executes) the string
	 */
	std::wstring interpret();
};

