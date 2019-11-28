#include "pch.h"
#include "token.h"

std::wstring token::value() const
{
	return _value;
}

token_type token::type() const
{
	return _type;
}

const wchar_t* token::token_type_to_string(token_type type)
{
	switch (type)
	{
	case token_type::plus:
		return L"plus";

	case token_type::minus:
		return L"min";

	case token_type::multiply:
		return L"mul";

	case token_type::divide_integer:
		return L"idiv";

	case token_type::divide_real:
		return L"rdiv";
		
	case token_type::eof:
		return L"end of file";

	case token_type::integer_const:
		return L"int";

	case token_type::real_const:
		return L"real";

	case token_type::group_start:
		return L"group_start";

	case token_type::group_end:
		return L"group_end";

	case token_type::program:
		return L"prog";

	case token_type::procedure:
		return L"procedure";

	case token_type::begin:
		return L"begin";
		
	case token_type::end:
		return L"end";
		
	case token_type::dot: 
		return L".";

	case token_type::assign:
		return L":=";

	case token_type::semicolon:
		return L"semi";

	case token_type::colon:
		return L"colon";

	case token_type::comma:
		return L"comma";
		
	case token_type::identifier:
		return L"idf";

	case token_type::integer_type:
		return L"int_t";
		
	case token_type::real_type:
		return L"real_t";

	case token_type::var_decl:
		return L"var_decl";
		
	default:
		return L"?";
	}
}

std::wstring token::to_string() const
{
	if (this->type() == token_type::eof)
	{
		return L"end of file";
	}
	
	const wchar_t* tokenType = token_type_to_string(this->type());

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

