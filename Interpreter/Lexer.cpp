#include "Lexer.h"
#include "interpret_except.h"

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
	
	if (this->pos >= this->input.size())
	{
		return Token::eof();
	}

	const auto currentChar = this->input[this->pos];

	if (isspace(currentChar))
	{
		this->pos += 1;
		return this->get_next_token();
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
