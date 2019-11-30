#pragma once
#include "ast_common.h"

class symbol;
using symbol_identifier = std::wstring;

using symbol_ptr = std::shared_ptr<symbol>;

template<typename T>
using symbol_type_ptr = std::shared_ptr<T>;

/* Helper function for making a pointer to AST node */
template <class T, class... TArgs>
symbol_ptr make_symbol_ptr(TArgs&&... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}

enum class symbol_type
{
	procedure,
	variable,
	type
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
	virtual std::wstring to_string() const = 0;

	const std::wstring& identifier() const;

	bool operator<(const symbol& rhs) const;
};

/**
 * Represents a built-in type
 */
class builtin_type_symbol final : public symbol
{
private:
	ast::builtin_type m_type;

public:
	explicit builtin_type_symbol(ast::builtin_type type);

	[[nodiscard]] ast::builtin_type type() const;

	static symbol_identifier var_type_to_string(ast::builtin_type type);
	
	[[nodiscard]] std::wstring to_string() const override;
};

/**
 * Represents a procedure
 */
class procedure_symbol final : public symbol
{
private:
	ast::procedure& m_procedure;

public:
	explicit procedure_symbol(ast::procedure& procedure);

	[[nodiscard]] const ast::procedure& procedure() const;
	[[nodiscard]] std::wstring to_string() const override;
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