#pragma once
#include "parser.h"
#include "memory_table.h"
#include "scope_context.h"

/**
 *
 * Interpret simple math expressions
 * 
 */
class interpreter
{
public:
	struct interpretation_info
	{
		std::shared_ptr<scope_context> global_scope;
		ast_ptr ast;
	};
	
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
	[[nodiscard]] std::wstring interpret_program();

	/**
	 * After interpretation, contains the global scope
	 * Primarily purpose is for interrogation in unit tests
	 */
	interpretation_info m_interpretation_info;
	
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

	/**
	 * Gets the global scope after interpretation
	 */
	[[nodiscard]] interpretation_info get_interpretation_info() const;
};

