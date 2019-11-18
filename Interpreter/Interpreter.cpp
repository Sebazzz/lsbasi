#include "Interpreter.h"
#include "interpret_except.h"
#include <stdexcept>
#include "math.h"
#include <optional>

void Interpreter::ensure_tokenized()
{
	if (this->tokens.empty())
	{
		this->do_tokenize();
	}
}

void Interpreter::compact_negative_integers()
{
	std::optional<Token> previousToken;
	for (size_t index=0; index<this->tokens.size();index++)
	{
		Token current = this->tokens[index];

		if (previousToken.has_value())
		{
			if (current.type() == TokenType::integer)
			{
				if (previousToken->type() == TokenType::minus)
				{
					// Previous is a minus-as-sign-specifier, so append and remove
					auto tokenVal = current.value();
					tokenVal.insert(0, 1, L'-');
					this->tokens[index] = Token(TokenType::integer, tokenVal);
					this->tokens.erase(this->tokens.begin() + index - 1);
					index--;
				}
			} else if (current.type() == TokenType::minus)
			{
				if (previousToken->type() != TokenType::integer)
				{
					// Previous is an operand, which means we should remember the current token
					previousToken.emplace(current);
					continue;
				}
			}

			previousToken.reset();
		} else
		{
			previousToken.emplace(current);
		}
	}
}

void Interpreter::do_tokenize()
{
	// Read all
	while (true)
	{
		Token current = this->lexer.get_next_token();
		if (current.type() == TokenType::eof)
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

std::wstring Interpreter::tokenize()
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

int Interpreter::get_integer(std::vector<Token>::iterator& token) const
{
	if (token == this->tokens.end())
	{
		throw interpret_except("Found end while searching for integer");
	}
	
	if (token->type() != TokenType::integer)
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

bool Interpreter::handle_operator(double& result, std::vector<Token>::iterator& it)
{
	if (this->tokens.end() == it)
	{
		return false;
	}

	const auto operatorType = it->type();
	++it;
	
	switch (operatorType)
	{
		case TokenType::plus:
			add_interpret(result, static_cast<double>(get_integer(it)));
			break;

		case TokenType::minus:
			subtract_interpret(result, static_cast<double>(get_integer(it)));
			break;

		case TokenType::multiply:
			// TODO: implement overflow detection
			result *= get_integer(it);
			break;

		case TokenType::divide:
			divide_interpret(result, static_cast<double>(get_integer(it)));
			break;

		default:
			throw interpret_except("Expected operator but found different token instead");
	}

	return true;
}

std::wstring Interpreter::interpret()
{
	this->ensure_tokenized();

	auto it = this->tokens.begin();
	double result = get_integer(it);

	while (handle_operator(result, it))
	{
		// Continue
	}

	return std::to_wstring(result);
}
