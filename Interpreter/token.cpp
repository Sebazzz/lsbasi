#include "token.h"

std::wstring token::value() const
{
	return _value;
}

token_type token::type() const
{
	return _type;
}

const wchar_t* token::getTokenTypeAsString() const
{
	switch (this->_type)
	{
	case token_type::plus:
		return L"plus";

	case token_type::minus:
		return L"min";

	case token_type::multiply:
		return L"mul";

	case token_type::divide:
		return L"div";
		
	case token_type::eof:
		return L"eof";

	case token_type::integer:
		return L"int";

	default:
		return L"?";
	}
}

std::wstring token::to_string() const
{
	const wchar_t* tokenType = getTokenTypeAsString();

	if (this->_value.empty())
	{
		return L"TOK(" + std::wstring(tokenType) + L")";
	}
	
	return L"TOK(" + std::wstring(tokenType) + L"," + std::wstring(this->_value) + L")";
}

token token::eof()
{
	return token(token_type::eof, std::wstring());
}
