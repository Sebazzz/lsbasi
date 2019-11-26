#pragma once
#include "lexer.h"
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
	
	std::vector<token> tokens;

	void ensure_tokenized();
	void do_tokenize();

	ast_ptr handle_integer(std::vector<token>::iterator& it) const;
	ast_ptr handle_unary(std::vector<token>::iterator& it) const;
	ast_ptr handle_factor(std::vector<token>::iterator& it) const;
	ast_ptr handle_group(std::vector<token>::iterator& it) const;
	ast_ptr handle_term(std::vector<token>::iterator& it) const;
	ast_ptr handle_expr(std::vector<token>::iterator& it) const;
	
	ast_ptr handle_program(std::vector<token>::iterator& it) const;
	void handle_var_decl_list(std::vector<token>::iterator& it, ast::var_decl_list& var_declaration_list) const;
	
	ast::ast_node_ptr<ast::block> handle_block(std::vector<token>::iterator& it) const;
	ast::compound_ptr handle_compound(std::vector<token>::iterator& it) const;
	void handle_statement_list(std::vector<token>::iterator& it, ast::statement_list& statement_list) const;
	
	ast_ptr handle_statement(std::vector<token>::iterator& it) const;
	ast_ptr handle_assign(std::vector<token>::iterator& it) const;

	inline bool is_at_end(std::vector<token>::iterator& it) const;
	
public:
	explicit parser(std::wstring input)
		: lexer(std::move(input))
	{
		this->tokens.reserve(3);
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

