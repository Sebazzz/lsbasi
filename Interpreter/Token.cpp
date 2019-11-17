#include "Token.h"

std::wstring Token::value() const
{
	return _value;
}

TokenType Token::type() const
{
	return _type;
}

const wchar_t* Token::getTokenTypeAsString() const
{
	switch (this->_type)
	{
	case TokenType::plus:
		return L"plus";

	case TokenType::minus:
		return L"minus";
		
	case TokenType::eof:
		return L"eof";

	case TokenType::integer:
		return L"integer";

	default:
		return L"?";
	}
}

std::wstring Token::to_string() const
{
	const wchar_t* tokenType = getTokenTypeAsString();

	const wchar_t* fmt = L"TOKEN(%s,%s)";
	if (this->_value.empty())
	{
		fmt = L"TOKEN(%s)";
	}

	// Format string
	const size_t sz = 256;
	wchar_t buffer[sz];
	swprintf_s(buffer, sz, fmt, tokenType, this->_value.c_str());
	
	return std::wstring(buffer);
}

Token Token::eof()
{
	return Token(TokenType::eof, std::wstring());
}
