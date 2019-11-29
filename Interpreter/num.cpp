#include "pch.h"
#include "num.h"
#include "ast_node_visitor.h"
#include "memory_table.h"

using namespace ast;

num::num(const int value): ast_node(token_type::integer_const), m_value(value), m_type(var_type::integer)
{
}

num::num(const double value): ast_node(token_type::integer_const), m_value(value), m_type(var_type::real)
{
}

num::num(num&& other) noexcept: ast_node(other),
                                m_value(other.m_value), m_type(other.m_type)
{
}

num& num::operator=(const num& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_value = other.m_value;
	m_type = other.m_type;
	return *this;
}

num& num::operator=(num&& other) noexcept
{
	if (this == &other)
		return *this;
	m_value = other.m_value;
	m_type = other.m_type;
	ast_node::operator =(other);
	return *this;
}

std::wstring num::val_to_string() const
{
	switch (this->type())
	{
	case ast::var_type::integer:
		return std::to_wstring(this->m_value.int_val);
	case ast::var_type::real:
		return std::to_wstring(this->m_value.real_val);
	default:
		throw interpret_except("Unsupported number type", std::to_string(static_cast<int>(this->type())))
		;
	}
}

expression_value num::value() const
{
	return this->m_value;
}

var_type num::type() const
{
	return this->m_type;
}

void num::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
