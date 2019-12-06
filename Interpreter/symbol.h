#pragma once
#include "ast_common.h"
#include "symbol_types.h"

enum class symbol_type
{
	procedure,
	variable,
	type
};

class type_impl;

/**
 * Base abstract class that represents a symbol
 */
class symbol abstract
{
private:
	symbol_type m_type;
	symbol_identifier m_identifier;

protected:
	symbol(symbol_type symbol, const symbol_identifier& identifier);

public:
	virtual ~symbol() = default;
	virtual std::wstring to_string() const = 0;

	const std::wstring& identifier() const;

	bool operator<(const symbol& rhs) const;
};

/**
 * Represents a variable
 */
class variable_symbol final : public symbol
{
private:
	ast::var_decl& m_var;

public:
	explicit variable_symbol(ast::var_decl& variable);

	[[nodiscard]] const ast::var_decl& variable() const;
	[[nodiscard]] std::wstring to_string() const override;
};