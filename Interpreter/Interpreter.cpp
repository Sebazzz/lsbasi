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

int Interpreter::parse_token_as_integer(const std::vector<Token>::value_type& token) const
{
	try
	{
		return std::stoi(token.value());
	} catch (std::invalid_argument& e)
	{
		throw interpret_except(e);
	} catch (std::out_of_range& e)
	{
		throw interpret_except(e);
	}
}

std::wstring Interpreter::interpret()
{
	this->ensure_tokenized();

	double result = 0;
	std::optional<Token> lastToken;

	for (const auto& token : this->tokens)
	{
		if (lastToken.has_value())
		{
			if (lastToken->type() != TokenType::integer)
			{
				// Current token must be integer
				if (token.type() != TokenType::integer)
				{
					throw interpret_except("Expect integer after operator");
				}

				// Execute operation
				switch (lastToken->type())
				{
				case TokenType::plus:
					add_interpret(result, static_cast<double>(parse_token_as_integer(token)));
					break;

				case TokenType::minus:
					subtract_interpret(result, static_cast<double>(parse_token_as_integer(token)));
					break;

				case TokenType::multiply:
					// TODO: implement overflow detection
					result *= parse_token_as_integer(token);
					break;

				case TokenType::divide:
					divide_interpret(result, static_cast<double>(parse_token_as_integer(token)));
					break;

				default:
					throw interpret_except("Invalid parser state handling integer");
				}
			} else
			{
				// Last token is integer, can't have an integer now too
				if (token.type() == TokenType::integer)
				{
					throw interpret_except("Two consequent integer tokens");
				}
			}
		} else
		{
			// No previous token, so we cannot have an operator now
			if (token.type() != TokenType::integer)
			{
				throw interpret_except("Operation without operand");
			}

			// Parse number
			result = parse_token_as_integer(token);
		}

		lastToken.emplace(token);
	}

	return std::to_wstring(result);
}
