#include "pch.h"
#include "lexer.h"
#include <map>
#include "util.h"

bool lexer::is_at_end() const
{
	return this->m_position >= this->input.size();
}

void lexer::advance()
{
	this->m_position++;

	if (this->is_at_end())
	{
		this->m_current_char = 0;
	} else
	{
		this->m_current_char = this->input[this->m_position];
	}
}

wchar_t lexer::peek() const
{
	const auto next_pos = this->m_position + 1;

	if (next_pos >= this->input.size())
	{
		return NO_NEXT_CHAR;
	}

	return this->input[next_pos];
}

bool lexer::skip_whitespace()
{
	bool hasSkippedWhitespace = false;
	while (!this->is_at_end() && isspace(this->m_current_char))
	{
		hasSkippedWhitespace = true;
		this->advance();
	}

	return hasSkippedWhitespace;
}

bool lexer::skip_comment()
{
	if (this->m_current_char != '{')
	{
		return false;
	}
	
	do
	{
		this->advance();
	} while (!this->is_at_end() && this->m_current_char != '}');
	
	this->advance();

	return true;
}

token lexer::read_digit()
{
	// This var holds our lexeme: what makes up our digit token
	std::wstring allDigits(1, this->m_current_char);

	token_type type = token_type::integer_const;

	while (!this->is_at_end())
	{
		this->advance();

		if (this->m_current_char == '.')
		{
			type = token_type::real_const;
		} else if (!isdigit(this->m_current_char))
		{
			break;
		}

		allDigits += this->m_current_char;
	}
	
	return token(type, allDigits);
}

token lexer::read_operator()
{
	switch (this->m_current_char)
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
		return token(token_type::divide_real, std::wstring());

	case '(':
		this->advance();
		return token(token_type::group_start, std::wstring());

	case ')':
		this->advance();
		return token(token_type::group_end, std::wstring());

	default:
		throw interpret_except("Unknown token in string: " + std::to_string(static_cast<char>(this->m_current_char)));
	}
}

token lexer::read_identifier_or_keyword()
{
	std::wstring identifier;

	// [A-z][A-z0-9]+
	while (!this->is_at_end() && (std::isalnum(this->m_current_char) || this->m_current_char == '_'))
	{
		identifier += this->m_current_char;
		this->advance();
	}

	static std::map<std::wstring, token, case_insensitive_string_comparer> reserved_keywords {
		{ L"PROGRAM", token(token_type::program, L"PROGRAM" )},
		{ L"PROCEDURE", token(token_type::procedure, L"PROCEDURE" )},
		{ L"INTEGER", token(token_type::identifier, L"INTEGER" )},
		{ L"REAL", token(token_type::identifier, L"REAL" )},
		{ L"VAR", token(token_type::var_decl, L"VAR" )},
		{ L"BEGIN", token(token_type::begin, L"BEGIN" )},
		{ L"DIV", token(token_type::divide_integer, L"DIV" )},
		{ L"END", token(token_type::end, L"END" )}
	};

	const auto reserved_keyword = reserved_keywords.find(identifier);
	if (reserved_keyword != reserved_keywords.end())
	{
		// Found a reserved keyword, return it
		return reserved_keyword->second;
	}

	return token(token_type::identifier, identifier);
}

lexer::lexer(std::wstring input): input(std::move(input))
{
	this->m_position = 0;
	this->m_current_char = !this->input.empty() ? this->input[this->m_position] : 0;
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

		if (this->skip_comment())
		{
			continue;
		}

		if (std::isalpha(this->m_current_char) || this->m_current_char == '_')
		{
			return this->read_identifier_or_keyword();
		}

		if (this->m_current_char == L':' && this->peek() == L'=')
		{
			this->advance();
			this->advance();
			return make_simple_token(token_type::assign);
		}

		if (this->m_current_char == L':')
		{
			this->advance();
			return make_simple_token(token_type::colon);
		}

		if (this->m_current_char == L';')
		{
			this->advance();
			return make_simple_token(token_type::semicolon);
		}

		if (this->m_current_char == L',')
		{
			this->advance();
			return make_simple_token(token_type::comma);
		}

		if (this->m_current_char == L'.')
		{
			this->advance();
			return make_simple_token(token_type::dot);
		}

		if (isdigit(this->m_current_char))
		{
			return this->read_digit();
		}

		return read_operator();
	}

	return token;
}
