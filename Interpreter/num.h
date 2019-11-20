#pragma once
#include "ast_node.h"
#include <string>
#include <utility>

class num :
	public ast_node
{
private:
	std::wstring m_value;

public:
	num(token_type token, std::wstring cs)
		: ast_node(token),
		  m_value(std::move(cs))
	{
	}

	num(const num& other) = default;

	num(num&& other) noexcept
		: ast_node(std::move(other)),
		  m_value(std::move(other.m_value))
	{
	}

	num& operator=(const num& other)
	{
		if (this == &other)
			return *this;
		ast_node::operator =(other);
		m_value = other.m_value;
		return *this;
	}

	num& operator=(num&& other) noexcept
	{
		if (this == &other)
			return *this;
		m_value = std::move(other.m_value);
		ast_node::operator =(std::move(other));
		return *this;
	}

	~num() = default;
};

