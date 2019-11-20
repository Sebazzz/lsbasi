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

	// We may just have gotten whitespace
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

int interpreter::handle_integer(std::vector<token>::iterator& it) const
{
	if (this->is_at_end(it))
	{
		throw interpret_except("Found end while searching for integer");
	}

	// We expect an integer. If it is prepended by a sign, it is negative
	int signCorrection = 1;
	if (it->type() == token_type::minus)
	{
		signCorrection = -1;
		++it;
	}

	if (this->is_at_end(it))
	{
		throw interpret_except("Expected integer");
	}

	// Now we don't expect a sign anymore.
	if (it->type() != token_type::integer)
	{
		throw interpret_except("Expected integer");
	}
	
	try
	{
		const auto val = std::stoi(it->value()) * signCorrection;
		++it;
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
	if (!this->is_at_end(it) && it->type() == token_type::group_end)
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

	while (!this->is_at_end(it))
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
	
	while (!this->is_at_end(it))
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

bool interpreter::is_at_end(std::vector<token>::iterator& it) const
{
	return it == this->tokens.end();
}

std::wstring interpreter::interpret()
{
	this->ensure_tokenized();

	auto it = this->tokens.begin();
	const double result = handle_expr(it);

	if (!this->is_at_end(it))
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
