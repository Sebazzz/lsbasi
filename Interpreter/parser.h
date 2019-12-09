#pragma once
#include "lexer.h"
#include "lexer_iterator.h"
#include "ast_common.h"
#include "compound.h"
#include "block.h"
#include <functional>
#include "interpreter_context.h"

// Grammar:
//
// program:         "PROGRAM" var SEMI block DOT
//
// block:           declarations compound
//
// declarations:    VAR (var_decl SEMI)+ |
//                  PROCEDURE ID (routine params)? SEMI block SEMI* |
//                  FUNCTION ID (routine params)? COLON type_spec SEMI block SEMI* |
//                  empty
//
// routine params: "(" var_decl (SEMI var_decl)+ ")"
//
// var_decl:        ID (COMMA ID) COLON type_spec
//
// type_spec:       "INTEGER" | "REAL"
//
// compound:        BEGIN statement_list END
//
// statement_list:  statement |  statement SEMI statement
//
// statement:       compound | assign | routine_call | if_statement | empty
//
// if_statement:    IF expression THEN statement (ELSE statement) |
//                  IF expression THEN compound (ELSE compound)
//
// routine_call:  ID "(" (expression (COMMA expression)*)? ")"
//
// assign:          ID ASSIGN expr
//
// empty:
// 
// expression:      term ((PLUS|MIN) term)*
// 
// term:            factor ((MUL|DIV) factor)*
// 
// factor:          (PLUS|MINUS) factor |
//                  integer |
//                  real |
//                  group |
//                  TRUE |
//                  FALSE |
//                  function_call |
//                  var
// 
// group:           "(" expression ")"

using ast_ptr = ast::ast_ptr;

class parser
{
private:
	lexer lexer;
	interpreter_context_ptr m_interpreter_context;
	
	ast_ptr handle_integer_or_literal(lexer_iterator& it) const;
	ast_ptr handle_unary(lexer_iterator& it) const;
	ast_ptr handle_factor(lexer_iterator& it) const;
	ast_ptr handle_group(lexer_iterator& it) const;
	ast_ptr handle_term(lexer_iterator& it) const;
	ast_ptr handle_expr(lexer_iterator& it) const;
	
	ast_ptr handle_program(lexer_iterator& it) const;
	std::shared_ptr<ast::routine> handle_procedure(lexer_iterator& it) const;
	std::shared_ptr<ast::routine> handle_function(lexer_iterator& it) const;
	void handle_var_decl_list(lexer_iterator& it, ast::var_decl_list& var_declaration_list) const;
	void handle_routine_decl_list(lexer_iterator& it, ast::routine_decl_list& procedure_declaration_list) const;

	/**
	 * Common routine for handling procedure params or variable declarations.
	 * Because the types are the same and the syntax is *almost* the same, the
	 * difference can be captured using custom logic via the end_of_decl_logic lambda.
	 */
	void handle_var_decl_or_parameter_list(lexer_iterator& it, ast::var_decl_list& var_declaration_list, std::function<void(lexer_iterator&)>& end_of_decl_logic) const;
	
	ast::ast_node_ptr<ast::block> handle_block(lexer_iterator& it) const;
	ast::compound_ptr handle_compound(lexer_iterator& it) const;
	void handle_statement_list(lexer_iterator& it, ast::statement_list& statement_list) const;
	
	ast_ptr handle_statement(lexer_iterator& it) const;

	ast_ptr handle_assign_or_procedure_call(lexer_iterator& it) const;
	ast_ptr handle_procedure_call(const std::wstring& identifier, lexer_iterator& it) const;
	ast_ptr handle_assign(const std::wstring& identifier, lexer_iterator& it) const;

public:
	explicit parser(lexer_input_stream input_stream, interpreter_context_ptr interpreter_context);

	/**
	 * Tokenizes the string and return string representation of the tokens
	 */
	std::wstring stringify_parse_tree();

	/**
	 * Parses the text as an abstract syntax tree (repl mode: no programs)
	 */
	ast_ptr parse_repl();

	/**
	 * Parses the text as an abstract syntax tree (expect full program)
	 */
	ast_ptr parse();
};

