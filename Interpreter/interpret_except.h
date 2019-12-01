#pragma once
#include "line_info.h"

/**
 * This exception is thrown when interpretation of the program fails
 */
class interpret_except: public std::exception
{
private:
	std::string m_message;
	line_info m_line_info{};

public:
	virtual ~interpret_except() = default;
	
	explicit interpret_except(const std::string& message, line_info line_info = { -1, -1 });
	explicit interpret_except(const std::wstring& message, line_info line_info = { -1, -1 });

	interpret_except(const interpret_except& other) = default;
	interpret_except(interpret_except&& other) noexcept;

	interpret_except& operator=(const interpret_except& other);
	interpret_except& operator=(interpret_except&& other) noexcept;

	[[nodiscard]] bool has_line_info() const;
	[[nodiscard]] line_info line_info() const;
	[[nodiscard]] char const* what() const override;
};

