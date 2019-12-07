#pragma once
#include "parser.h"
#include "memory_table.h"
#include "scope_context.h"
#include "interpreter_context.h"

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
		interpreter_context_ptr interpreter_context;
		ast_ptr ast;
	};
	
private:
	interpreter_context_ptr m_context_ptr;
	parser m_parser;
	
	ast_ptr m_parsed_ast;
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
	void interpret_program();

	/**
	 * After interpretation, contains the global scope
	 * Primarily purpose is for interrogation in unit tests
	 */
	interpretation_info m_interpretation_info;
	
public:
	explicit interpreter(lexer_input_stream input_stream, bool repl_mode = false);

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

