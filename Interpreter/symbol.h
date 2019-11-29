#pragma once
#include "ast_common.h"

class symbol;
using symbol_identifier = std::wstring;
using symbol_ptr = std::shared_ptr<symbol>;

/* Helper function for making a pointer to AST node */
template <class T, class... TArgs>
symbol_ptr make_symbol_ptr(TArgs&&... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}

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

	const std::wstring& identifier() const;

	bool operator<(const symbol& rhs) const;
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
	std::wstring to_string() override;
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
	std::wstring to_string() override;
};