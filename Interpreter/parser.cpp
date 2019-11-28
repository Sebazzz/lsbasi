#include "pch.h"
#include "parser.h"
#include "num.h"
#include "bin_op.h"
#include "unary_op.h"
#include "empty.h"
#include "var.h"
#include "assign.h"
#include "program.h"
#include "var_decl.h"

using namespace ast;

std::wstring parser::stringify_parse_tree()
{
    lexer_iterator it(this->lexer);
	
	std::wstring buffer;

	it.advance();
	while (!it.is_at_end())
	{
		buffer.append(it->to_string());
		buffer.append(L" ");
		
		it.advance();
	}
	
	return buffer;
}

ast_ptr parser::parse_repl()
{
    lexer_iterator it(this->lexer);
	it.advance();
	
	const ast_ptr result = handle_expr(it);
	
	if (!it.is_at_end())
	{
		// If we have still tokens left, we hit an unexpected token
		throw interpret_except("Unexpected token found", it->to_string());
	}

	return result;
}

ast_ptr parser::parse()
{
    lexer_iterator it(this->lexer);
	it.advance();
	
	const ast_ptr result = handle_program(it);

	if (!it.is_at_end())
	{
		// If we have still tokens left, we hit an unexpected token
		throw interpret_except("Unexpected token found at end of program", it->to_string());
	}

	return result;
}

ast_ptr parser::handle_integer(lexer_iterator& it) const
{
	it.ensure_token("Found end while searching for integer");

	try
	{
		switch (it->type())
		{
		case token_type::integer_const:
			{
				const auto val_i = std::stoi(it->value());
				it.advance();
				return make_ast_ptr<num>(val_i);
			}

		case token_type::real_const:
			{
				const auto val_r = std::stod(it->value());
				it.advance();
				return make_ast_ptr<num>(val_r);
			}
			
		default:
			throw interpret_except("Expected integer or real", it->to_string());
		}
	} catch (std::invalid_argument& e)
	{
		throw interpret_except(e);
	} catch (std::out_of_range& e)
	{
		throw interpret_except(e);
	}
}

ast_ptr parser::handle_unary(lexer_iterator& it) const
{
	const auto tokenType = it->type();
	if (tokenType != token_type::plus && tokenType != token_type::minus)
	{
		throw interpret_except("Expected unary operation");
	}

	it.advance();
	return make_ast_ptr<unary_op>(tokenType, handle_factor(it));
}

ast_ptr parser::handle_factor(lexer_iterator& it) const
{
	it.ensure_token("Found end while searching for factor");
	
	if (it->type() == token_type::group_start)
	{
		return handle_group(it);
	}

	if (it->type() == token_type::identifier)
	{
		const auto identifier = make_ast_node_ptr<var>(it->value());
		it.advance();
		return identifier;
	}

	if (it->type() == token_type::plus || it->type() == token_type::minus)
	{
		return handle_unary(it);
	}

	return handle_integer(it);
}

ast_ptr parser::handle_group(lexer_iterator& it) const
{
	it.skip_required(token_type::group_start); // Skip the current "start group" token

	const ast_ptr result = this->handle_expr(it);

	it.skip_required(token_type::group_end);
	
	return result;
}

ast_ptr parser::handle_term(lexer_iterator& it) const
{
	// We expect a factor with possibly a multiply or divide
	ast_ptr result = handle_factor(it);

	while (!it.is_at_end())
	{
		const auto operatorType = it->type();
		switch (operatorType)
		{
			case token_type::multiply:
			case token_type::divide_integer:
			case token_type::divide_real: // FIXME: until we implement real division
				it.advance();
				result = make_ast_ptr<bin_op>(result, operatorType, handle_factor(it));
				break;

			// Not a multiply or divide, just continue
			default:
				return result;
		}
	}
	
	return result;
}

ast_ptr parser::handle_expr(lexer_iterator& it) const
{
	ast_ptr result = handle_term(it);
	
	while (!it.is_at_end())
	{
		const auto operatorType = it->type();
		switch (operatorType)
		{
			case token_type::plus:
			case token_type::minus:
				it.advance();
				result = make_ast_ptr<bin_op>(result, operatorType, handle_term(it));
				break;

			default:
				// Note this may be an unexpected token
				return result;
		}
	}

	return result;
}

ast_ptr parser::handle_program(lexer_iterator& it) const
{
	// PROGRAM
	it.skip_required(token_type::program);

	// variable
	program_identifier identifier = it.expect(token_type::identifier).value();
	it.advance();

	// block
	it.skip_required(token_type::semicolon);
	
	auto block = this->handle_block(it);
	it.skip_required(token_type::dot);
	
	if (!it.is_at_end())
	{
		throw interpret_except("Expected end of program", it->to_string());
	}

	return make_ast_ptr<program>(identifier, block);
}

ast::ast_node_ptr<ast::block> parser::handle_block(lexer_iterator& it) const
{
	var_decl_list variable_declaration_list;
	handle_var_decl_list(it, variable_declaration_list);

	const auto compound = handle_compound(it);
	
	return make_ast_node_ptr<block>(variable_declaration_list, compound);
}

void parser::handle_var_decl_list(lexer_iterator& it, ast::var_decl_list& var_declaration_list) const
{
	if (it->type() != token_type::var_decl)
	{
		// This is not a variable declaration. Lets return control and hope the next parsing step knows what to do with it.
		return;
	}

	it.advance();

	std::vector<var_identifier> identifiers;
	do
	{
		// Get the identifiers
		identifiers.clear();
		do
		{
			auto identifier = it.expect(token_type::identifier).value();
			it.advance();

			identifiers.push_back(identifier);

			if (it->type() == token_type::comma)
			{
				it.advance();
			}

			// Yes, this allows a trailing comma. I call that a language _feature_ :)
		}while (it->type() != token_type::colon);

		it.skip_required(token_type::colon);

		// Get the type
		var_type v_type;
		switch (it->type())
		{
			case token_type::integer_type:
				v_type = var_type::integer;
			break;
			case token_type::real_type:
				v_type = var_type::real;
			break;
			default: throw interpret_except("Expected type specification", it->to_string());
		}

		for (auto && identifier : identifiers)
		{
			var_declaration_list.push_back(
				make_ast_node_ptr<var_decl>(identifier, v_type)
			);
		}

		it.advance();
		it.skip_required(token_type::semicolon);
	} while (!it.is_at_end() && it->type() != token_type::begin);
}

compound_ptr parser::handle_compound(lexer_iterator& it) const
{
	it.skip_required(token_type::begin);

	statement_list statement_list;
	handle_statement_list(it, statement_list);

	it.skip_required(token_type::end);

	return make_ast_node_ptr<compound>(statement_list);
}

void parser::handle_statement_list(lexer_iterator& it, statement_list& statement_list) const
{
	while (true)
	{
		statement_list.push_back(this->handle_statement(it));

		if (it.is_at_end())
		{
			// We will fail further up the call stack, but ignore this in this function
			return;
		}

		if (it->type() != token_type::semicolon)
		{
			// No next statement
			return;
		}

		// Prepare for next statement
		it.advance();
	}
	
}

ast_ptr parser::handle_statement(lexer_iterator& it) const
{
	it.ensure_token("Found end while searching for statement");

	if (it->type() == token_type::begin)
	{
		// Compound
		return this->handle_compound(it);
	}

	if (it->type() == token_type::end)
	{
		// Empty
		return make_ast_ptr<empty>();
	}

	if (it->type() == token_type::identifier)
	{
		// Assignment
		return this->handle_assign(it);
	}

	throw interpret_except("Unexpected token in statement", it->to_string());
}

ast_ptr parser::handle_assign(lexer_iterator& it) const
{
	it.ensure_token("Found end while handling assignment");

	const auto identifier = make_ast_node_ptr<var>(it->value());
	it.advance();

	it.skip_required(token_type::assign);

	// We could just do this check further down the stack, however,
	// if we handle it here, the error message is more clear
	it.ensure_token("Expected expression in assignment");
	
	const auto expression = this->handle_expr(it);

	return make_ast_ptr<assign>(identifier, expression);
}