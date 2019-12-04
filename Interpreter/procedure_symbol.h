#pragma once
#include "symbol.h"
#include "procedure.h"

/**
 * Represents a procedure
 */
class procedure_symbol abstract : public symbol
{
public:
	explicit procedure_symbol(const symbol_identifier& identifier);
	~procedure_symbol() override = default;
	
	[[nodiscard]] std::wstring to_string() const override;

	[[nodiscard]] virtual const ast::procedure_param_list& params() const = 0;
	[[nodiscard]] virtual symbol_table& symbol_table() const = 0;
};

/**
 * Represents a user-defined procedure
 */
class user_defined_procedure_symbol final : public procedure_symbol
{
private:
	ast::procedure& m_procedure;
	
public:
	explicit user_defined_procedure_symbol(ast::procedure& procedure);
	
	[[nodiscard]] const ast::procedure& procedure() const;

	[[nodiscard]] const ast::procedure_param_list& params() const override;
	[[nodiscard]] ::symbol_table& symbol_table() const override;
};