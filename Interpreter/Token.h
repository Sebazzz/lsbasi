#pragma once
#include "TokenType.h"
#include <string>
#include <utility>

class Token
{
private:
	const TokenType _type;
	const std::wstring _value;

public:
	[[nodiscard]] std::wstring value() const;

	[[nodiscard]] TokenType type() const;
	const wchar_t* getTokenTypeAsString() const;

	Token(TokenType token, std::wstring cs)
		: _type(token),
		  _value(std::move(cs))
	{
	}

	[[nodiscard]] std::wstring to_string() const;

	static Token eof();
};

