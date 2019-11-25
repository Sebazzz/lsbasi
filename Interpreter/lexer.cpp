#include "lexer.h"
#include "interpret_except.h"
#include <map>
#include "util.h"

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

	return this->input[next_pos];
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

static std::map<std::wstring, token, case_insensitive_string_comparer> reserved_keywords {
	{ L"BEGIN", token(token_type::begin, L"BEGIN" )},
	{ L"END", token(token_type::end, L"END" )}
};

token lexer::read_identifier_or_keyword()
{
	std::wstring identifier;

	// [A-z][A-z0-9]+
	while (!this->is_at_end() && (std::isalnum(this->currentChar) || this->currentChar == '_'))
	{
		identifier += this->currentChar;
		this->advance();
	}

	const auto reserved_keyword = reserved_keywords.find(identifier);
	if (reserved_keyword != reserved_keywords.end())
	{
		// Found a reserved keyword, return it
		return reserved_keyword->second;
	}

	return token(token_type::identifier, identifier);
}

token make_simple_token(token_type type)
{
	return token(type, std::wstring());
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

		if (std::isalpha(this->currentChar) || this->currentChar == '_')
		{
			return this->read_identifier_or_keyword();
		}

		if (this->currentChar == L':' && this->peek() == L'=')
		{
			this->advance();
			this->advance();
			return make_simple_token(token_type::assign);
		}

		if (this->currentChar == L';')
		{
			this->advance();
			return make_simple_token(token_type::semicolon);
		}

		if (this->currentChar == L'.')
		{
			this->advance();
			return make_simple_token(token_type::dot);
		}

		if (isdigit(this->currentChar))
		{
			return this->read_digit();
		}

		return read_operator();
	}

	return token;
}
