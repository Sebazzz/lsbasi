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

	case TokenType::multiply:
		return L"mul";

	case TokenType::divide:
		return L"div";
		
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

	if (this->_value.empty())
	{
		return L"TOKEN(" + std::wstring(tokenType) + L")";
	}
	
	return L"TOKEN(" + std::wstring(tokenType) + L"," + std::wstring(this->_value) + L")";
}

Token Token::eof()
{
	return Token(TokenType::eof, std::wstring());
}
