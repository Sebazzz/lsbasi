#pragma once
#include "lexer.h"
#include "ast_node.h"
#include <vector>


// Grammar:
//
// expression: term ((PLUS|MIN)term)*
// term:       factor ((MUL|DIV) factor)*
// factor:     (PLUS|MINUS)factor | integer | group
// group:      "(" expression ")"

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
	 * Parses the text as an abstract syntax tree
	 */
	ast_ptr parse();
};

