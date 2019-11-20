#pragma once
#include "ast_node.h"
#include <string>

class num :
	public ast_node
{
private:
	std::wstring m_value;

public:
	num(token_type token, std::wstring cs);

	num(const num& other) = default;

	num(num&& other) noexcept;

	num& operator=(const num& other);

	num& operator=(num&& other) noexcept;

	~num() = default;

	[[nodiscard]] std::wstring value() const;
};

