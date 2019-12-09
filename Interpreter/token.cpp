#include "pch.h"
#include "token.h"

std::wstring token::value() const
{
	return m_value;
}

token_type token::type() const
{
	return m_type;
}

token::token(token_type token, std::wstring contents, ::line_info line_info): m_type(token),
                                                                              m_value(std::move(contents)),
                                                                              m_position(line_info)
{
}

token::token(token_type token, ::line_info line_info): m_type(token), m_position(line_info)
{
}

token::token(token&& other) noexcept: m_type(other.m_type), m_value(std::move(other.m_value)),
                                      m_position(other.m_position)
{
}

token& token::operator=(const token& other)
{
	if (this == &other)
		return *this;
	m_type = other.m_type;
	m_position = other.m_position;
	m_value = other.m_value;
	return *this;
}

token& token::operator=(token&& other) noexcept
{
	if (this == &other)
		return *this;
	m_type = other.m_type;
	m_position = other.m_position;
	m_value = std::move(other.m_value);
	return *this;
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

	case token_type::compare_equal:
		return L"eq";
		
	case token_type::eof:
		return L"end of file";

	case token_type::integer_const:
		return L"int";

	case token_type::real_const:
		return L"real";

	case token_type::string_const:
		return L"str";

	case token_type::if_ctrl:
		return L"if";

	case token_type::then_ctrl:
		return L"then";

	case token_type::else_ctrl:
		return L"else";

	case token_type::group_start:
		return L"group_start";

	case token_type::group_end:
		return L"group_end";

	case token_type::boolean_const:
		return L"bool";

	case token_type::program:
		return L"prog";

	case token_type::procedure:
		return L"procedure";

	case token_type::function:
		return L"function";

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

	if (this->m_value.empty())
	{
		return L"token(" + std::wstring(tokenType) + L")";
	}
	
	return L"token(" + std::wstring(tokenType) + L"," + std::wstring(this->m_value) + L")";
}

line_info token::position() const
{
	return m_position;
}

token token::eof(line_info position)
{
	return token(token_type::eof, position);
}

