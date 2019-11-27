#pragma once
#include "token_type.h"
#include <string>
#include <utility>

class token
{
private:
	token_type _type;
	std::wstring _value;
	
public:
	[[nodiscard]] std::wstring value() const;

	[[nodiscard]] token_type type() const;

	token(token_type token, std::wstring cs)
		: _type(token),
		  _value(std::move(cs))
	{
	}

	token(const token& other) = default;

	token(token&& other) noexcept
		: _type(other._type),
		  _value(std::move(other._value))
	{
	}

	token& operator=(const token& other)
	{
		if (this == &other)
			return *this;
		_type = other._type;
		_value = other._value;
		return *this;
	}

	token& operator=(token&& other) noexcept
	{
		if (this == &other)
			return *this;
		_type = other._type;
		_value = std::move(other._value);
		return *this;
	}

	[[nodiscard]] std::wstring to_string() const;

	static token eof();

	static const wchar_t* token_type_to_string(token_type type);
};

