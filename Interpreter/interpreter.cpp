#include "interpreter.h"
#include "interpret_except.h"
#include <stdexcept>
#include "math.h"
#include <optional>

void interpreter::ensure_tokenized()
{
	if (this->tokens.empty())
	{
		this->do_tokenize();
	}
}

void interpreter::compact_negative_integers()
{
	// Make from subsequent tokens "-" and "<number>" a negative number token
	
	for (size_t index=0; index<this->tokens.size();index++)
	{
		token current = this->tokens[index];

		// If this isn't an integer we can't negate
		if (current.type() != token_type::integer)
			continue;

		// If there isn't a previous token, or it isn't a minus we cannot negate
		if (index < 1 || this->tokens[index-1].type() != token_type::minus)
			continue;
		
		if (index == 1 || index >= 2 && (this->tokens[index-2].type() != token_type::integer && this->tokens[index-2].type() != token_type::group_end))
		{
			// Previous is a minus-as-sign-specifier, so append and remove
			auto tokenVal = current.value();
			tokenVal.insert(0, 1, L'-');
			this->tokens[index] = token(token_type::integer, tokenVal);
			this->tokens.erase(this->tokens.begin() + index - 1);
			index--;
		}
	}
}

void interpreter::do_tokenize()
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

	// Optimize
	compact_negative_integers();
	
	if (this->tokens.empty())
	{
		throw interpret_except("No tokens were parsed");
	}
}

std::wstring interpreter::tokenize()
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

int interpreter::handle_integer(std::vector<token>::iterator& token) const
{
	if (token == this->tokens.end())
	{
		throw interpret_except("Found end while searching for integer");
	}
	
	if (token->type() != token_type::integer)
	{
		throw interpret_except("Expected integer");
	}
	
	try
	{
		const auto val = std::stoi(token->value());
		++token;
		return val;
	} catch (std::invalid_argument& e)
	{
		throw interpret_except(e);
	} catch (std::out_of_range& e)
	{
		throw interpret_except(e);
	}
}

double interpreter::handle_factor(std::vector<token>::iterator& it) const
{
	if (it->type() == token_type::group_start)
	{
		return handle_group(it);
	}

	return handle_integer(it);
}

double interpreter::handle_group(std::vector<token>::iterator& it) const
{
	++it; // Skip the current "start group" token

	const double result = this->handle_expr(it);
	if (it != this->tokens.end() && it->type() == token_type::group_end)
	{
		++it;
		return result;
	}

	throw interpret_except("Expected to find end-of-group");
}

double interpreter::handle_term(std::vector<token>::iterator& it) const
{
	// We expect a factor with possibly a multiply or divide
	double result = handle_factor(it);

	while (it != this->tokens.end())
	{
		switch (it->type())
		{
			case token_type::multiply:
				++it;
			
				// TODO: implement overflow detection
				result *= handle_factor(it);
				break;

			case token_type::divide:
				++it;
				divide_interpret(result, static_cast<double>(handle_factor(it)));
				break;

			// Not a multiply or divide, just continue
			default:
				return result;
		}
	}
	
	return result;
}

double interpreter::handle_expr(std::vector<token>::iterator& it) const
{
	double result = handle_term(it);
	
	while (it != this->tokens.end())
	{
		const auto operatorType = it->type();
		switch (operatorType)
		{
			case token_type::plus:
				++it;
				add_interpret(result, static_cast<double>(handle_term(it)));
				break;

			case token_type::minus:
				++it;
				subtract_interpret(result, static_cast<double>(handle_term(it)));
				break;

			default:
				// Note this may be an unexpected token
				return result;
		}
	}

	return result;
}

std::wstring interpreter::interpret()
{
	this->ensure_tokenized();

	auto it = this->tokens.begin();
	const double result = handle_expr(it);

	if (it != this->tokens.end())
	{
		// If we have still tokens left, we hit an unexpected token
		throw interpret_except("Unexpected token found");
	}

	if (round(result) == result)
	{
		return std::to_wstring(static_cast<int>(result));
	}

	return std::to_wstring(result);
}
