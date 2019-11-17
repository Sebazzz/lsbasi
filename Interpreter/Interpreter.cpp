#include "Interpreter.h"
#include "interpret_except.h"
#include <stdexcept>

void Interpreter::tokenizeIfNecessary()
{
	if (this->tokens.empty())
	{
		this->tokenizeCore();
	}
}

void Interpreter::tokenizeCore()
{
	if (this->input.empty())
	{
		throw interpret_except("string is empty");
	}

	while (true)
	{
		Token current = this->getNextToken();
		if (current.type() == TokenType::eof)
		{
			break;
		}

		this->tokens.push_back(current);
	}

	if (this->input.empty())
	{
		throw interpret_except("No tokens were parsed");
	}
}

/*
    Lexical analyzer (also known as scanner or tokenizer)

    This method is responsible for breaking a sentence
    apart into tokens. One token at a time.
 */
Token Interpreter::getNextToken()
{
	if (this->pos >= this->input.size())
	{
		return Token::eof();
	}

	const auto currentChar = this->input[this->pos];

	if (isspace(currentChar))
	{
		this->pos += 1;
		return this->getNextToken();
	}
	
	if (isdigit(currentChar))
	{
		this->pos += 1;
		return Token(TokenType::integer, std::wstring(1, currentChar));
	}

	switch (currentChar)
	{
	case '+':
		this->pos += 1;
		return Token(TokenType::plus, std::wstring());

	default:
		throw interpret_except("Unknown token in string");
	}
}

std::wstring Interpreter::tokenize()
{
	this->tokenizeIfNecessary();

	std::wstring buffer;
	for (const auto& token : this->tokens)
	{
		buffer.append(token.to_string());
		buffer.append(L" ");
	}

	return buffer;
}

int Interpreter::parseIntegerToken(const std::vector<Token>::value_type& token) const
{
	try
	{
		return std::stoi(token.value());
	} catch (std::invalid_argument& e)
	{
		throw interpret_except(e);
	} catch (std::overflow_error& e)
	{
		throw interpret_except(e);
	}
}

std::wstring Interpreter::interpret()
{
	this->tokenizeIfNecessary();

	int result = 0;
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
					result += parseIntegerToken(token);
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
			result = parseIntegerToken(token);
		}

		lastToken.emplace(token);
	}

	return std::to_wstring(result);
}
