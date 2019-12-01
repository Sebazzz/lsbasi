#include "pch.h"

interpret_except::interpret_except(const std::string& message, ::line_info line_info) : m_line_info(line_info)
{
	m_message = message;
}

interpret_except::interpret_except(const std::wstring& message, ::line_info line_info) : m_line_info(line_info)
{
	m_message = wstring_to_string(message);
}

interpret_except::interpret_except(interpret_except&& other) noexcept: std::exception(std::move(other)),
                                                                       m_message(std::move(other.m_message)),
                                                                       m_line_info(std::move(other.m_line_info))
{
}

interpret_except& interpret_except::operator=(const interpret_except& other)
{
	if (this == &other)
		return *this;
	std::exception::operator =(other);
	m_message = other.m_message;
	m_line_info = other.m_line_info;
	return *this;
}

interpret_except& interpret_except::operator=(interpret_except&& other) noexcept
{
	if (this == &other)
		return *this;
	std::exception::operator =(std::move(other));
	m_message = std::move(other.m_message);
	m_line_info = std::move(other.m_line_info);
	return *this;
}

bool interpret_except::has_line_info() const
{
	return this->m_line_info.line_number > 0;
}

line_info interpret_except::line_info() const
{
	return this->m_line_info;
}

char const* interpret_except::what() const
{
	return this->m_message.c_str();
}
