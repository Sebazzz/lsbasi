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
	// Handle newlines
	// ... Note we don't handle a carriage return (\r), but since in  Windows CRLF streams the \n will always be present, we can just use the LF
	if (this->m_current_char == '\n')
	{
		this->m_stream_position.line_number++;
		this->m_stream_position.column = 0;
	}
	
	this->m_position++;
	this->m_stream_position.column++;

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
	const auto stream_pos = this->m_stream_position;
	
	// This var holds our lexeme: what makes up our digit token
	std::wstring all_digits(1, this->m_current_char);

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

		all_digits += this->m_current_char;
	}
	
	return token(type, all_digits, stream_pos);
}

token lexer::read_operator()
{
	const auto stream_pos = this->m_stream_position;
	
	switch (this->m_current_char)
	{
	case '+':
		this->advance();
		return token(token_type::plus, stream_pos);

	case '-':
		this->advance();
		return token(token_type::minus, stream_pos);

	case '*':
		this->advance();
		return token(token_type::multiply, stream_pos);

	case '/':
		this->advance();
		return token(token_type::divide_real, stream_pos);

	case '(':
		this->advance();
		return token(token_type::group_start, stream_pos);

	case ')':
		this->advance();
		return token(token_type::group_end, stream_pos);

	default:
		throw interpret_except("Unknown token in string: " + std::to_string(static_cast<char>(this->m_current_char)));
	}
}

token lexer::read_identifier_or_keyword()
{
	const auto stream_pos = this->m_stream_position;

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
		return token_with_line_info(reserved_keyword->second, stream_pos);
	}

	return token(token_type::identifier, identifier, stream_pos);
}

token lexer::token_with_line_info(const token& static_token, const line_info& position)
{
	return token(static_token.type(), static_token.value(), position);
}

lexer::lexer(std::wstring input): input(std::move(input)), m_stream_position({1, 1})
{
	this->m_position = 0;
	this->m_current_char = !this->input.empty() ? this->input[this->m_position] : 0;
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
	
	auto result_token = token::eof();
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
			const auto stream_pos = this->m_stream_position;
			this->advance();
			this->advance();
			return token(token_type::assign, stream_pos);
		}

		if (this->m_current_char == L':')
		{
			const auto stream_pos = this->m_stream_position;
			this->advance();
			return token(token_type::colon, stream_pos);
		}

		if (this->m_current_char == L';')
		{
			const auto stream_pos = this->m_stream_position;
			this->advance();
			return token(token_type::semicolon, stream_pos);
		}

		if (this->m_current_char == L',')
		{
			const auto stream_pos = this->m_stream_position;
			this->advance();
			return token(token_type::comma, stream_pos);
		}

		if (this->m_current_char == L'.')
		{
			const auto stream_pos = this->m_stream_position;
			this->advance();
			return token(token_type::dot, stream_pos);
		}

		if (isdigit(this->m_current_char))
		{
			return this->read_digit();
		}

		return read_operator();
	}

	return result_token;
}
