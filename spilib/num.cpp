#include "pch.h"
#include "num.h"
#include "ast_node_visitor.h"
#include "memory_table.h"

using namespace ast;

num::num(bool value, token token): ast_node(std::move(token)), m_value(value), m_type(builtin_type::boolean)
{
}

num::num(const int value, token token): ast_node(std::move(token)), m_value(value), m_type(builtin_type::integer)
{
}

num::num(builtin_string_ptr value, token token): ast_node(std::move(token)), m_value(value), m_type(builtin_type::string)
{
}

num::num(const double value, token token): ast_node(std::move(token)), m_value(value), m_type(builtin_type::real)
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
	case builtin_type::integer:
		return std::to_wstring(this->m_value.int_val);
	case builtin_type::real:
		return std::to_wstring(this->m_value.real_val);
	case builtin_type::string:
		return *this->m_value.string_ptr_val;
	default:
		throw internal_interpret_except("Unsupported number type: " + std::to_string(static_cast<int>(this->type())), this->get_line_info())
		;
	}
}

expression_value num::value() const
{
	return this->m_value;
}

builtin_type num::type() const
{
	return this->m_type;
}

void num::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}
