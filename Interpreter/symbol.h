#pragma once
#include "ast_common.h"

using symbol_identifier = std::wstring;

enum class symbol_type
{
	procedure,
	variable
};

/**
 * Base abstract class that represents a symbol
 */
class symbol abstract
{
private:
	symbol_type m_type;
	symbol_identifier m_identifier;

protected:
	symbol(symbol_type symbol, const symbol_identifier& cs);

public:
	virtual ~symbol() = default;
	virtual std::wstring to_string() = 0;

	bool operator<(const symbol& rhs) const;
};

/**
 * Represents a procedure
 */
class procedure_symbol : public symbol
{
private:
	ast::procedure_decl_ptr m_procedure;

public:
	explicit procedure_symbol(ast::procedure_decl_ptr procedure);
};

/**
 * Represents a variable
 */
class variable_symbol : public symbol
{
private:
	ast::var_decl_ptr m_var;

public:
	explicit variable_symbol(ast::var_decl_ptr variable);
};