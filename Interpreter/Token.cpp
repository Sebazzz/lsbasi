#include "Token.h"

std::wstring Token::value() const
{
	return _value;
}

TokenType Token::type() const
{
	return _type;
}

std::wstring Token::to_string() const
{
	const wchar_t* tokenType;
	switch (this->_type)
	{
	case TokenType::plus:
		tokenType = L"plus";
		break;

	case TokenType::eof:
		tokenType = L"eof";
		break;

	case TokenType::integer:
		tokenType = L"integer";
		break;

	default:
		tokenType = L"?";
		break;
	}

	const size_t sz = 256;
	wchar_t buffer[sz];
	swprintf_s(buffer, sz, L"TOKEN(%s,%s)", tokenType, this->_value.c_str());
	return std::wstring(buffer);
}

Token Token::eof()
{
	return Token(TokenType::eof, std::wstring());
}
