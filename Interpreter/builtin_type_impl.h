#pragma once
#include "type_impl.h"
#include "builtin_type_symbol.h"

class builtin_type_impl final : public type_impl
{
private:
	/*
	 * Raw pointer because this instance is owned by the symbol
	 */
	builtin_type_symbol* m_symbol;

	void assign_self_type(eval_value& eval_value) const;
	void convert_value(ast::expression_value& expr_value, builtin_type_symbol& expr_type, line_info line_info) const;

public:
	explicit builtin_type_impl(builtin_type_symbol* builtin_type_symbol);

	virtual ~builtin_type_impl() = default;
	builtin_type_impl(const builtin_type_impl& other) = delete;
	builtin_type_impl(builtin_type_impl&& other) noexcept = delete;
	builtin_type_impl& operator=(const builtin_type_impl& other) = delete;
	builtin_type_impl& operator=(builtin_type_impl&& other) noexcept = delete;
	
	void convert_value(eval_value& eval_value, line_info line_info) const override;
	void change_type(eval_value& eval_value, line_info line_info) const override;
};