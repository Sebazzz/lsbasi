#include "Lexer.h"
#include "interpret_except.h"

bool Lexer::is_at_end() const
{
	return this->pos >= this->input.size();
}

void Lexer::advance()
{
	this->pos++;

	if (this->is_at_end())
	{
		this->currentChar = 0;
	} else
	{
		this->currentChar = this->input[this->pos];
	}
}

bool Lexer::skip_whitespace()
{
	bool hasSkippedWhitespace = false;
	while (!this->is_at_end() && isspace(this->currentChar))
	{
		hasSkippedWhitespace = true;
		this->advance();
	}

	return hasSkippedWhitespace;
}

Token Lexer::read_digit()
{
	// This var holds our lexeme: what makes up our digit token
	std::wstring allDigits(1, this->currentChar);

	while (!this->is_at_end())
	{
		this->advance();
		
		if (!isdigit(this->currentChar))
		{
			break;
		}

		allDigits += this->currentChar;
	}
	
	return Token(TokenType::integer, allDigits);
}

Token Lexer::read_operator()
{
	switch (this->currentChar)
	{
	case '+':
		this->advance();
		return Token(TokenType::plus, std::wstring());

	case '-':
		this->advance();
		return Token(TokenType::minus, std::wstring());

	case '*':
		this->advance();
		return Token(TokenType::multiply, std::wstring());

	case '/':
		this->advance();
		return Token(TokenType::divide, std::wstring());

	default:
		throw interpret_except("Unknown token in string");
	}
}

/*
    Lexical analyzer (also known as scanner or tokenizer)

    This method is responsible for breaking a sentence
    apart into tokens. One token at a time.
 */
Token Lexer::get_next_token()
{
	if (this->input.empty())
	{
		throw interpret_except("string is empty");
	}
	
	auto token = Token::eof();
	while (!this->is_at_end())
	{
		if (this->skip_whitespace())
		{
			continue;
		}

		if (isdigit(this->currentChar))
		{
			return this->read_digit();
		}

		return read_operator();
	}

	return token;
}
