#pragma once
#include "symbol.h"
#include "procedure.h"

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
