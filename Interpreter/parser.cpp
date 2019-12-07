#include "pch.h"
#include "parser.h"
#include "num.h"
#include "bin_op.h"
#include "unary_op.h"
#include "empty.h"
#include "assignment_target.h"
#include "assign.h"
#include "program.h"
#include "routine.h"
#include "var_decl.h"
#include <functional>
#include <utility>
#include "routine_call.h"

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
		throw parse_except(L"Unexpected token found: " + it->to_string(), it->position());
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
		throw parse_except(L"Unexpected token found at end of program" + it->to_string(), it->position());
	}

	return result;
}

ast_ptr parser::handle_integer_or_literal(lexer_iterator& it) const
{
	it.ensure_token("Found end while searching for integer");

	const auto num_str = it->value();
	const auto token = it.current_token();
	
	try
	{
		switch (it->type())
		{
		case token_type::integer_const:
			{
				// Disable auto: let's be explicit
				// ReSharper disable CppUseAuto
				const builtin_integer val_i = std::stoi(num_str);
				// ReSharper restore CppUseAuto
				
				it.advance();
				return make_ast_ptr<num>(val_i, token);
			}

		case token_type::real_const:
			{
				// Disable auto: let's be explicit
				// ReSharper disable CppUseAuto
				const builtin_real val_r = std::stod(num_str);
				// ReSharper restore CppUseAuto
				
				it.advance();
				return make_ast_ptr<num>(val_r, token);
			}

		case token_type::string_const:
			{
				// Pool the internal string
				builtin_string_ptr str_ptr = this->m_interpreter_context->string_pool.get_or_add(num_str);
				
				it.advance();

				// We can essentially clear the internal string and replace it with a token,
				// I don't believe the string value is actually used somewhere else
				return make_ast_ptr<num>(str_ptr, ::token(token.type(), L"POOLED", token.position()));
			}
			
		default:
			throw parse_except(L"Expected string literal, integer or real: " + it->to_string(), it->position());
		}
	} catch (std::invalid_argument& e)
	{
		throw parse_except(L"Unable to parse integer or real from string '" + num_str + L"'" + string_to_wstring(e.what()), it->position());
	} catch (std::out_of_range& e)
	{
		throw parse_except(L"Unable to parse integer or real from string '" + num_str + L"' - number is too large" + string_to_wstring(e.what()), it->position());
	}
}

ast_ptr parser::handle_unary(lexer_iterator& it) const
{
	const auto token = it.current_token();
	const auto tokenType = it->type();
	if (tokenType != token_type::plus && tokenType != token_type::minus)
	{
		throw parse_except("Expected unary operation", it->position());
	}

	it.advance();
	return make_ast_ptr<unary_op>(tokenType, handle_factor(it), token);
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
		const auto identifier_token = it.current_token();
		it.advance();

		if (!it.is_at_end() && it->type() == token_type::group_start)
		{
			// This must be an function call
			return this->handle_procedure_call(identifier_token.value(), it);
		}

		const auto var_node = make_ast_ptr<assignment_target>(identifier_token.value(), identifier_token);
		return var_node;
	}

	if (it->type() == token_type::plus || it->type() == token_type::minus)
	{
		return handle_unary(it);
	}

	return handle_integer_or_literal(it);
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

	const auto token = it.current_token();
	while (!it.is_at_end())
	{
		const auto operatorType = it->type();
		switch (operatorType)
		{
			case token_type::multiply:
			case token_type::divide_integer:
			case token_type::divide_real:
				it.advance();
				result = make_ast_ptr<bin_op>(result, operatorType, handle_factor(it), token);
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
	const auto token = it.current_token();
	
	while (!it.is_at_end())
	{
		const auto operatorType = it->type();
		switch (operatorType)
		{
			case token_type::plus:
			case token_type::minus:
				it.advance();
				result = make_ast_ptr<bin_op>(result, operatorType, handle_term(it), token);
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
	const auto token = it.expect(token_type::program);
	it.advance();

	// variable
	program_identifier identifier = it.expect(token_type::identifier).value();
	it.advance();

	// block
	it.skip_required(token_type::semicolon);
	
	auto block = this->handle_block(it);
	it.skip_required(token_type::dot);
	
	if (!it.is_at_end())
	{
		throw parse_except(L"Expected end of program" + it->to_string(), it->position());
	}

	return make_ast_ptr<program>(identifier, block, token);
}

ast_node_ptr<routine> parser::handle_procedure(lexer_iterator& it) const
{
	const auto token = it.expect(token_type::procedure);
	it.advance();

	// ID
	const auto procedure_id = it.expect(token_type::identifier).value();
	it.advance();

	// Possible parameter list
	it.ensure_token("Expect semicolon or procedure parameter list");

	procedure_param_list param_list;
	if (it->type() == token_type::group_start)
	{
		it.advance();

		std::function<void(lexer_iterator&)> skip_logic = [](lexer_iterator& it)
		{
			// Skip semicolons, but formal parameters don't end with one so we cannot require it
			if (it.current_token().type() == token_type::semicolon)
			{
				it.advance();
			}
		};
		this->handle_var_decl_or_parameter_list(it, param_list, skip_logic);

		it.skip_required(token_type::group_end);
	}
	
	it.skip_required(token_type::semicolon);

	// Expect block
	auto block = this->handle_block(it);

	it.skip_required(token_type::semicolon);

	return make_ast_ptr<routine>(procedure_id, param_list, block, token);
}

std::shared_ptr<ast::routine> parser::handle_function(lexer_iterator& it) const
{
	const auto token = it.expect(token_type::function);
	it.advance();

	// ID
	const auto procedure_id = it.expect(token_type::identifier).value();
	it.advance();

	// Possible parameter list
	it.ensure_token("Expect semicolon or procedure parameter list");

	procedure_param_list param_list;
	if (it->type() == token_type::group_start)
	{
		it.advance();

		std::function<void(lexer_iterator&)> skip_logic = [](lexer_iterator& it)
		{
			// Skip semicolons, but formal parameters don't end with one so we cannot require it
			if (it.current_token().type() == token_type::semicolon)
			{
				it.advance();
			}
		};
		this->handle_var_decl_or_parameter_list(it, param_list, skip_logic);

		it.skip_required(token_type::group_end);
	}

	// Type spec
	it.skip_required(token_type::colon);

	const auto type_token = it.expect(token_type::identifier);
	const auto return_type = make_ast_ptr<type>(type_token.value(), type_token);
	it.advance();
	it.skip_required(token_type::semicolon);

	// Expect block
	auto block = this->handle_block(it);

	it.skip_required(token_type::semicolon);

	return make_ast_ptr<routine>(procedure_id, param_list, return_type, block, token);
}

ast_node_ptr<block> parser::handle_block(lexer_iterator& it) const
{
	const auto token = it.current_token();
	
	var_decl_list variable_declaration_list;
	handle_var_decl_list(it, variable_declaration_list);

	routine_decl_list procedure_declaration_list;
	handle_routine_decl_list(it, procedure_declaration_list);

	const auto compound = handle_compound(it);
	
	return make_ast_ptr<block>(variable_declaration_list, procedure_declaration_list, compound, token);
}

void parser::handle_var_decl_list(lexer_iterator& it, var_decl_list& var_declaration_list) const
{
	if (it->type() != token_type::var_decl)
	{
		// This is not a variable declaration. Lets return control and hope the next parsing step knows what to do with it.
		return;
	}

	it.advance();

	std::function<void(lexer_iterator&)> skip_logic = [](lexer_iterator& _it) { _it.skip_required(token_type::semicolon); };
	this->handle_var_decl_or_parameter_list(it, var_declaration_list, skip_logic);
}

void parser::handle_var_decl_or_parameter_list(lexer_iterator& it, var_decl_list& var_declaration_list, std::function<void(lexer_iterator&)>& end_of_decl_logic) const
{
	std::vector<std::pair<var_identifier, token>> identifiers;
	do
	{
		// Get the identifiers
		identifiers.clear();
		do
		{
			auto identifier_token = it.expect(token_type::identifier);
			it.advance();

			identifiers.emplace_back(identifier_token.value(), identifier_token);

			if (it->type() == token_type::comma)
			{
				it.advance();
			}

			// Yes, this allows a trailing comma. I call that a language _feature_ :)
		}while (it->type() != token_type::colon);

		it.skip_required(token_type::colon);

		// Get the type
		type_identifier v_type;
		const auto type_token = it.current_token();
		switch (it->type())
		{
			case token_type::identifier:
				v_type = it->value();
			break;
			default: throw parse_except(L"Expected type specification: " + it->to_string(), it->position());
		}

		for (auto && identifier : identifiers)
		{
			var_declaration_list.push_back(
				make_ast_ptr<var_decl>(identifier.first, make_ast_ptr<type>(v_type, type_token), identifier.second)
			);
		}

		it.advance();
		end_of_decl_logic(it);	
	} while (!it.is_at_end() && it->type() == token_type::identifier);
}

void parser::handle_routine_decl_list(lexer_iterator& it, routine_decl_list& procedure_declaration_list) const
{
	if (it->type() != token_type::procedure && it->type() != token_type::function)
	{
		// This is not a procedure or function declaration. Lets return control and hope the next parsing step knows what to do with it.
		return;
	}

	do
	{
		if (it->type() == token_type::procedure)
		{
			auto procedure = this->handle_procedure(it);
			procedure_declaration_list.push_back(procedure);
		} else if (it ->type() == token_type::function)
		{
			auto function = this->handle_function(it);
			procedure_declaration_list.push_back(function);
		}
	} while (!it.is_at_end() && (it->type() == token_type::procedure || it->type() == token_type::function));
}

compound_ptr parser::handle_compound(lexer_iterator& it) const
{
	const auto token = it.expect(token_type::begin);
	it.advance();

	statement_list statement_list;
	handle_statement_list(it, statement_list);

	it.skip_required(token_type::end);

	return make_ast_ptr<compound>(statement_list, token);
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
		return make_ast_ptr<empty>(it.current_token());
	}

	if (it->type() == token_type::identifier)
	{
		// Assignment
		return this->handle_assign_or_procedure_call(it);
	}

	throw parse_except(L"Unexpected token in statement: " + it->to_string(), it->position());
}

ast_ptr parser::handle_assign_or_procedure_call(lexer_iterator& it) const
{
	// Take identifier
	const std::wstring identifier = it.expect(token_type::identifier).value();
	it.advance();

	// It could go two ways here: We have an assignment or we call a procedure.
	// This depends on whether we have a assignment operator or opening group.

	it.ensure_token("Found end while handling assignment or procedure call");
	if (it->type() == token_type::assign)
	{
		// Assignment
		return this->handle_assign(identifier, it);
	}

	if (it->type() == token_type::group_start)
	{
		// Parameter list of procedure call
		return this->handle_procedure_call(identifier, it);
	}
	
	throw parse_except(L"Unexpected token in statement attempting to parse assignment or procedure call: " + it->to_string(), it->position());
}

ast_ptr parser::handle_procedure_call(const std::wstring& identifier, lexer_iterator& it) const
{
	const auto token = it.expect(token_type::group_start);
	it.advance();
	it.ensure_token("Expected parameter list for procedure call");

	// The parameters for a procedure is a comma delimited list of expressions
	procedure_arg_list arg_list;
	while (!it.is_at_end() && it->type() != token_type::group_end)
	{
		arg_list.push_back(this->handle_expr(it));

		if (it->type() != token_type::group_end)
		{
			it.skip_required(token_type::comma);
		}
	}

	it.skip_required(token_type::group_end);
	it.ensure_token("Found end of file while parsing procedure argument list");

	return make_ast_ptr<routine_call>(identifier, arg_list, token);
}

ast_ptr parser::handle_assign(const std::wstring& identifier, lexer_iterator& it) const
{
	const auto var_identifier = make_ast_ptr<assignment_target>(identifier, it.current_token());
	
	const auto token = it.expect(token_type::assign);
	it.advance();

	// We could just do this check further down the stack, however,
	// if we handle it here, the error message is more clear
	it.ensure_token("Expected expression in assignment");
	
	const auto expression = this->handle_expr(it);

	return make_ast_ptr<assign>(var_identifier, expression, token);
}

parser::parser(lexer_input_stream input_stream, interpreter_context_ptr interpreter_context): lexer(input_stream), m_interpreter_context(std::move(interpreter_context))
{
}
