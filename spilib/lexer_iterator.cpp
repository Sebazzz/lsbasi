#include "pch.h"
#include "lexer_iterator.h"

void lexer_iterator::skip_required(token_type type)
{
	this->expect_internal(type);
	this->advance();
}

void lexer_iterator::advance()
{
	this->m_previous_position = this->m_current_token.position();
	this->m_current_token = this->m_lexer.get_next_token();
}

const token& lexer_iterator::current_token() const noexcept
{
	return this->m_current_token;
}

void lexer_iterator::expect_internal(token_type type) const
{
	if (this->is_at_end())
	{
		throw parse_except(std::wstring(L"At the end of the file while expecting token: ") + token::token_type_to_string(type), this->m_previous_position);
	}

	if (this->m_current_token.type() != type)
	{
		std::wstring info_str;
		info_str += L"Expected type ";
		info_str += token::token_type_to_string(type);
		info_str += L"; Got token: ";
		info_str += this->m_current_token.to_string();
		
		throw parse_except(L"Invalid token found: " + info_str, this->m_current_token.position());
	}
}

lexer_iterator::lexer_iterator(lexer& lexer): m_lexer(lexer), m_current_token(token_type::eof, L""), m_previous_position({-1,-1})
{
}

const token& lexer_iterator::expect(token_type type) const
{
	this->expect_internal(type);
	return this->m_current_token;
}

const token* lexer_iterator::operator->() const
{
	if (this->is_at_end())
	{
		throw parse_except("Unexpected end of file while searching for token", this->m_previous_position);
	}

	return &this->m_current_token;
}

bool lexer_iterator::is_at_end() const
{
	return this->m_current_token.type() == token_type::eof;
}

void lexer_iterator::ensure_token(const std::string& except_message) const
{
	if (this->is_at_end())
	{
		throw parse_except(except_message, this->m_previous_position);
	}
}
