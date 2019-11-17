#include "Interpreter.h"
#include "interpret_except.h"
#include <stdexcept>
#include "math.h"

void Interpreter::tokenizeIfNecessary()
{
	if (this->tokens.empty())
	{
		this->tokenizeCore();
	}
}

void Interpreter::handleNegativeIntegers()
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

void Interpreter::tokenizeCore()
{
	if (this->input.empty())
	{
		throw interpret_except("string is empty");
	}

	// Read all
	while (true)
	{
		Token current = this->getNextToken();
		if (current.type() == TokenType::eof)
		{
			break;
		}

		this->tokens.push_back(current);
	}

	// Optimize
	handleNegativeIntegers();
	
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

		std::wstring allDigits(1, currentChar);

		{
			wchar_t possibleDigit;
			while (this->pos < this->input.size() && isdigit(possibleDigit = this->input[this->pos]))
			{
				allDigits += possibleDigit;
				this->pos++;
			}
		}
		
		return Token(TokenType::integer, allDigits);
	}

	switch (currentChar)
	{
	case '+':
		this->pos += 1;
		return Token(TokenType::plus, std::wstring());

	case '-':
		this->pos += 1;
		return Token(TokenType::minus, std::wstring());

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
	} catch (std::out_of_range& e)
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
					add_interpret(result, parseIntegerToken(token));
					break;

				case TokenType::minus:
					subtract_interpret(result, parseIntegerToken(token));
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
