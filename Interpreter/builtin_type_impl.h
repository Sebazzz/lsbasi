#pragma once
#include "type_impl.h"
#include "builtin_type_symbol.h"

class builtin_type_impl abstract : public type_impl
{
protected:
	/*
	 * Raw pointer because this instance is owned by the symbol
	 */
	builtin_type_symbol* m_symbol;
	
	virtual void assign_self_type(eval_value& eval_value) const;
	virtual void convert_value(ast::expression_value& expr_value, builtin_type_symbol& expr_type, line_info line_info) const = 0;
	
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

class builtin_real_impl final : public builtin_type_impl
{
protected:
	void convert_value(ast::expression_value& expr_value, builtin_type_symbol& expr_type, line_info line_info) const override;
	
public:
	explicit builtin_real_impl(builtin_type_symbol* builtin_type_symbol);

	virtual ~builtin_real_impl() = default;
	builtin_real_impl(const builtin_real_impl& other) = delete;
	builtin_real_impl(builtin_real_impl&& other) noexcept = delete;
	builtin_real_impl& operator=(const builtin_real_impl& other) = delete;
	builtin_real_impl& operator=(builtin_real_impl&& other) noexcept = delete;
};


class builtin_integer_impl final : public builtin_type_impl
{
protected:
	void convert_value(ast::expression_value& expr_value, builtin_type_symbol& expr_type, line_info line_info) const override;
	
public:
	explicit builtin_integer_impl(builtin_type_symbol* builtin_type_symbol);

	virtual ~builtin_integer_impl() = default;
	builtin_integer_impl(const builtin_integer_impl& other) = delete;
	builtin_integer_impl(builtin_integer_impl&& other) noexcept = delete;
	builtin_integer_impl& operator=(const builtin_integer_impl& other) = delete;
	builtin_integer_impl& operator=(builtin_integer_impl&& other) noexcept = delete;
};

class builtin_string_impl final : public builtin_type_impl
{
protected:
	void convert_value(ast::expression_value& expr_value, builtin_type_symbol& expr_type, line_info line_info) const override;
	
public:
	explicit builtin_string_impl(builtin_type_symbol* builtin_type_symbol);

	virtual ~builtin_string_impl() = default;
	builtin_string_impl(const builtin_string_impl& other) = delete;
	builtin_string_impl(builtin_string_impl&& other) noexcept = delete;
	builtin_string_impl& operator=(const builtin_string_impl& other) = delete;
	builtin_string_impl& operator=(builtin_string_impl&& other) noexcept = delete;
};