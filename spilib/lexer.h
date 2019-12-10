#pragma once
#include "token.h"

using lexer_input_traits = std::char_traits<wchar_t>;
using lexer_input_stream = std::basic_istream<wchar_t, lexer_input_traits>&;

class lexer final
{
	/**
	 * Returned by peek method to indicate that we reached the end
	 */
	constexpr static wchar_t NO_NEXT_CHAR = 0;
	
	// Constant
	lexer_input_stream m_input_stream;

	// State variables
	wchar_t m_current_char;
	line_info m_stream_position;

	[[nodiscard]] bool is_at_end() const;
	void advance();

	/**
	 * Peeks the next character or returns NO_NEXT_CHAR on failure
	 */
	[[nodiscard]] wchar_t peek() const; // Considered returning a std::optional instead, but a constant works just as well

	bool skip_whitespace();
	bool skip_comment();

	token read_string();
	token read_digit();
	token read_operator();
	token read_identifier_or_keyword();

	/**
	 * Utility function to create token with line info from a predefined token.
	 */
	static token token_with_line_info(const token& static_token, const line_info& position);
	
	// Seems quite wasteful to return a token but the optimizing compiler will transform this into a tail call.

public:
	explicit lexer(lexer_input_stream input_stream);

	/**
	 * Reads the next token from the string
	 */
	token get_next_token();
};

