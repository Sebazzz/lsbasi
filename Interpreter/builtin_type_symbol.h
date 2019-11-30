#pragma once
#include "type_symbol.h"

class builtin_type_impl;

/**
 * Represents a built-in type
 */
class builtin_type_symbol final : public type_symbol
{
private:
	ast::builtin_type m_type;
	
	std::unique_ptr<builtin_type_impl> m_type_impl;

public:
	explicit builtin_type_symbol(ast::builtin_type type);

	[[nodiscard]] ast::builtin_type type() const;

	static symbol_identifier var_type_to_string(ast::builtin_type type);
	
	[[nodiscard]] std::wstring to_string() const override;

	const ::type_impl& type_impl() override;

	static symbol_type_ptr<type_symbol> get_for_builtin_type(ast::builtin_type type_spec);
};