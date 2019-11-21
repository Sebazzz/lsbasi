#include "lexer.h"
#include "interpret_except.h"

bool lexer::is_at_end() const
{
	return this->pos >= this->input.size();
}

void lexer::advance()
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

wchar_t lexer::peek() const
{
	const auto next_pos = this->pos + 1;

	if (next_pos >= this->input.size())
	{
		return NO_NEXT_CHAR;
	}

	return this->input[this->pos];
}

bool lexer::skip_whitespace()
{
	bool hasSkippedWhitespace = false;
	while (!this->is_at_end() && isspace(this->currentChar))
	{
		hasSkippedWhitespace = true;
		this->advance();
	}

	return hasSkippedWhitespace;
}

token lexer::read_digit()
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
	
	return token(token_type::integer, allDigits);
}

token lexer::read_operator()
{
	switch (this->currentChar)
	{
	case '+':
		this->advance();
		return token(token_type::plus, std::wstring());

	case '-':
		this->advance();
		return token(token_type::minus, std::wstring());

	case '*':
		this->advance();
		return token(token_type::multiply, std::wstring());

	case '/':
		this->advance();
		return token(token_type::divide, std::wstring());

	case '(':
		this->advance();
		return token(token_type::group_start, std::wstring());

	case ')':
		this->advance();
		return token(token_type::group_end, std::wstring());

	default:
		throw interpret_except("Unknown token in string");
	}
}

/*
    Lexical analyzer (also known as scanner or tokenizer)

    This method is responsible for breaking a sentence
    apart into tokens. One token at a time.
 */
token lexer::get_next_token()
{
	if (this->input.empty())
	{
		throw interpret_except("string is empty");
	}
	
	auto token = token::eof();
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
