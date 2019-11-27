#pragma once
#include <string>
#include <utility>
#include <memory>
#include "lexer.h"
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
	bool m_repl_mode;

	void ensure_parsed();
	void do_parse();

	/**
	 * Interpret in REPL mode
	 */
	[[nodiscard]] std::wstring interpret_repl() const;

	/**
	 * Interpret, expecting full program
	 */
	[[nodiscard]] std::wstring interpret_program() const;


public:
	explicit interpreter(std::wstring input, bool repl_mode = false)
		: parser(std::move(input)), m_repl_mode(repl_mode)
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

