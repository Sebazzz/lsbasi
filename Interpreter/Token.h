#pragma once
#include "TokenType.h"
#include <string>
#include <utility>

class Token
{
private:
	TokenType _type;
	std::wstring _value;
	
	[[nodiscard]] const wchar_t* getTokenTypeAsString() const;

public:
	[[nodiscard]] std::wstring value() const;

	[[nodiscard]] TokenType type() const;

	Token(TokenType token, std::wstring cs)
		: _type(token),
		  _value(std::move(cs))
	{
	}


	Token(const Token& other)
		: _type(other._type),
		  _value(other._value)
	{
	}

	Token(Token&& other) noexcept
		: _type(other._type),
		  _value(std::move(other._value))
	{
	}

	Token& operator=(const Token& other)
	{
		if (this == &other)
			return *this;
		_type = other._type;
		_value = other._value;
		return *this;
	}

	Token& operator=(Token&& other) noexcept
	{
		if (this == &other)
			return *this;
		_type = other._type;
		_value = std::move(other._value);
		return *this;
	}

	[[nodiscard]] std::wstring to_string() const;

	static Token eof();
};

