#include "num.h"

num::num(token_type token, std::wstring cs): ast_node(token),
                                             m_value(std::move(cs))
{
}

num::num(num&& other) noexcept: ast_node(std::move(other)),
                                m_value(std::move(other.m_value))
{
}

num& num::operator=(const num& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_value = other.m_value;
	return *this;
}

num& num::operator=(num&& other) noexcept
{
	if (this == &other)
		return *this;
	m_value = std::move(other.m_value);
	ast_node::operator =(std::move(other));
	return *this;
}

std::wstring num::value() const
{
	return m_value;
}
