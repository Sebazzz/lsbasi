#pragma once
#include "lexer.h"
#include "lexer_iterator.h"
#include "ast_node.h"
#include <vector>
#include "compound.h"
#include "block.h"

// Grammar:
//
// program:         "PROGRAM" var SEMI block DOT
//
// block:           declarations compound
//
// declarations:    VAR (var_decl SEMI) | empty
//
// var:             ID (COMMA ID) COLON type_spec
//
// type_spec:       "INTEGER" | "REAL"
//
// compound:        BEGIN statement_list END
//
// statement_list:  statement |  statement SEMI statement
//
// statement:       compound | assign | empty
//
// assign:          var ASSIGN expr
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
//                  var
// 
// group:           "(" expression ")"

using ast_ptr = ast::ast_ptr;

class parser
{
private:
	lexer lexer;
	
	void do_tokenize();

	ast_ptr handle_integer(lexer_iterator& it) const;
	ast_ptr handle_unary(lexer_iterator& it) const;
	ast_ptr handle_factor(lexer_iterator& it) const;
	ast_ptr handle_group(lexer_iterator& it) const;
	ast_ptr handle_term(lexer_iterator& it) const;
	ast_ptr handle_expr(lexer_iterator& it) const;
	
	ast_ptr handle_program(lexer_iterator& it) const;
	void handle_var_decl_list(lexer_iterator& it, ast::var_decl_list& var_declaration_list) const;
	
	ast::ast_node_ptr<ast::block> handle_block(lexer_iterator& it) const;
	ast::compound_ptr handle_compound(lexer_iterator& it) const;
	void handle_statement_list(lexer_iterator& it, ast::statement_list& statement_list) const;
	
	ast_ptr handle_statement(lexer_iterator& it) const;
	ast_ptr handle_assign(lexer_iterator& it) const;

public:
	explicit parser(std::wstring input)
		: lexer(std::move(input))
	{
	}

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

