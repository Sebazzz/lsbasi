#include "parser.h"
#include "interpret_except.h"
#include <stdexcept>
#include "num.h"
#include "bin_op.h"
#include "unary_op.h"

using namespace ast;

std::wstring parser::stringify_parse_tree()
{
	this->ensure_tokenized();

	std::wstring buffer;
	for (const auto& token : this->tokens)
	{
		buffer.append(token.to_string());
		buffer.append(L" ");
	}

	return buffer;
}

ast_ptr parser::parse_repl()
{
	this->ensure_tokenized();

	auto it = this->tokens.begin();
	const ast_ptr result = handle_expr(it);
	
	if (!this->is_at_end(it))
	{
		// If we have still tokens left, we hit an unexpected token
		throw interpret_except("Unexpected token found");
	}

	return result;
}

ast_ptr parser::parse()
{
	this->ensure_tokenized();

	auto it = this->tokens.begin();
	const ast_ptr result = handle_program(it);

	if (!this->is_at_end(it))
	{
		// If we have still tokens left, we hit an unexpected token
		throw interpret_except("Unexpected token found at end of program");
	}

	return result;
}

void parser::ensure_tokenized()
{
	if (this->tokens.empty())
	{
		this->do_tokenize();
	}
}

void parser::do_tokenize()
{
	// Read all
	while (true)
	{
		token current = this->lexer.get_next_token();
		if (current.type() == token_type::eof)
		{
			break;
		}

		this->tokens.push_back(current);
	}

	// We may just have gotten whitespace
	if (this->tokens.empty())
	{
		throw interpret_except("No tokens were parsed");
	}
}

ast_ptr parser::handle_integer(std::vector<token>::iterator& it) const
{
	if (this->is_at_end(it))
	{
		throw interpret_except("Found end while searching for integer");
	}

	if (it->type() != token_type::integer)
	{
		throw interpret_except("Expected integer");
	}
	
	try
	{
		const auto val = std::stoi(it->value());
		++it;
		return make_ast_ptr<num>(val);
	} catch (std::invalid_argument& e)
	{
		throw interpret_except(e);
	} catch (std::out_of_range& e)
	{
		throw interpret_except(e);
	}
}

ast_ptr parser::handle_unary(std::vector<token>::iterator& it) const
{
	const auto tokenType = it->type();
	if (tokenType != token_type::plus && tokenType != token_type::minus)
	{
		throw interpret_except("Expected unary operation");
	}

	++it;
	return make_ast_ptr<unary_op>(tokenType, handle_factor(it));
}

ast_ptr parser::handle_factor(std::vector<token>::iterator& it) const
{
	if (this->is_at_end(it))
	{
		throw interpret_except("Found end while searching for factor");
	}
	
	if (it->type() == token_type::group_start)
	{
		return handle_group(it);
	}

	if (it->type() == token_type::plus || it->type() == token_type::minus)
	{
		return handle_unary(it);
	}

	return handle_integer(it);
}

ast_ptr parser::handle_group(std::vector<token>::iterator& it) const
{
	++it; // Skip the current "start group" token

	const ast_ptr result = this->handle_expr(it);
	if (!this->is_at_end(it) && it->type() == token_type::group_end)
	{
		++it;
		return result;
	}

	throw interpret_except("Expected to find end-of-group");
}

ast_ptr parser::handle_term(std::vector<token>::iterator& it) const
{
	// We expect a factor with possibly a multiply or divide
	ast_ptr result = handle_factor(it);

	while (!this->is_at_end(it))
	{
		const auto operatorType = it->type();
		switch (operatorType)
		{
			case token_type::multiply:
			case token_type::divide:
				++it;
				result = make_ast_ptr<bin_op>(result, operatorType, handle_factor(it));
				break;

			// Not a multiply or divide, just continue
			default:
				return result;
		}
	}
	
	return result;
}

ast_ptr parser::handle_expr(std::vector<token>::iterator& it) const
{
	ast_ptr result = handle_term(it);
	
	while (!this->is_at_end(it))
	{
		const auto operatorType = it->type();
		switch (operatorType)
		{
			case token_type::plus:
			case token_type::minus:
				++it;
				result = make_ast_ptr<bin_op>(result, operatorType, handle_term(it));
				break;

			default:
				// Note this may be an unexpected token
				return result;
		}
	}

	return result;
}

ast_ptr parser::handle_program(std::vector<token>::iterator&) const
{
	throw std::logic_error("Not implemented");
}

ast_ptr parser::handle_compound(std::vector<token>::iterator&) const
{
	throw std::logic_error("Not implemented");
}

ast_ptr parser::handle_statement_list(std::vector<token>::iterator&, ast::statement_list&) const
{
	throw std::logic_error("Not implemented");
}

ast_ptr parser::handle_statement(std::vector<token>::iterator&) const
{
	throw std::logic_error("Not implemented");
}

ast_ptr parser::handle_assign(std::vector<token>::iterator&) const
{
	throw std::logic_error("Not implemented");
}

bool parser::is_at_end(std::vector<token>::iterator& it) const
{
	return it == this->tokens.end();
}
