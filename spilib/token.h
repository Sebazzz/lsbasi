#pragma once
#include "token_type.h"
#include "line_info.h"

/**
 * Represents a single lexeme in the parsing stream
 */
class token final
{
private:
	token_type m_type;
	std::wstring m_value;
	line_info m_position;
	
public:
	[[nodiscard]] std::wstring value() const;

	[[nodiscard]] token_type type() const;

	token(token_type token, std::wstring contents, line_info line_info = {-1, -1});

	token(token_type token, line_info line_info = { -1, -1});

	~token() = default;
	
	token(const token& other) = default;

	token(token&& other) noexcept;

	token& operator=(const token& other);

	token& operator=(token&& other) noexcept;

	[[nodiscard]] std::wstring to_string() const;
	[[nodiscard]] line_info position() const;

	static token eof(line_info position = {-1, -1});

	static const wchar_t* token_type_to_string(token_type type);
};

