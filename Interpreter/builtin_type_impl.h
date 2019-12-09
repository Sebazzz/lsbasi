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
	
	virtual void execute_binary_operation(eval_value& current, const ast::expression_value& from, const builtin_type_symbol& right_val, token_type op, type_operation_context& type_operation_context) const = 0;

	/**
	 * Helper function to copy the symbol type pointer
	 */
	virtual void assign_self_type(eval_value& eval_value, type_operation_context& type_operation_context) const;
	
	/**
	 * Gets if the current type supports conversion from the specific type
	 */
	virtual bool supports_type_conversion_from(ast::builtin_type from_type, token_type op) const = 0;

	/**
	 * Converts the specified value to the current type
	 */
	virtual void implicit_type_conversion(ast::expression_value& value, builtin_type_symbol* builtin_type, type_operation_context& type_operation_context) const;

public:
	explicit builtin_type_impl(builtin_type_symbol* builtin_type_symbol);

	virtual ~builtin_type_impl() = default;
	builtin_type_impl(const builtin_type_impl& other) = delete;
	builtin_type_impl(builtin_type_impl&& other) noexcept = delete;
	builtin_type_impl& operator=(const builtin_type_impl& other) = delete;
	builtin_type_impl& operator=(builtin_type_impl&& other) noexcept = delete;

	/**
	 * Gets if the current type supports conversion from the specific type
	 */
	bool supports_implicit_type_conversion_from(symbol_type_ptr<type_symbol> type, token_type op) const override;

	void implicit_type_conversion(eval_value& value, type_operation_context& type_operation_context) const override;
	
	void execute_binary_operation(eval_value& result, const eval_value& right_val, token_type op, type_operation_context& type_context) const override;
};

class builtin_real_impl final : public builtin_type_impl
{
protected:
	[[nodiscard]] bool supports_type_conversion_from(ast::builtin_type from_type, token_type op) const override;

	void execute_binary_operation(eval_value& result, const ast::expression_value& from, const builtin_type_symbol& right_val, token_type op, type_operation_context& type_operation_context) const override;

	void implicit_type_conversion(ast::expression_value& value, builtin_type_symbol* builtin_type, type_operation_context& type_operation_context) const override;
	
public:
	explicit builtin_real_impl(builtin_type_symbol* builtin_type_symbol);

	virtual ~builtin_real_impl() = default;
	builtin_real_impl(const builtin_real_impl& other) = delete;
	builtin_real_impl(builtin_real_impl&& other) noexcept = delete;
	builtin_real_impl& operator=(const builtin_real_impl& other) = delete;
	builtin_real_impl& operator=(builtin_real_impl&& other) noexcept = delete;
};

class builtin_boolean_impl final : public builtin_type_impl
{
protected:
	[[nodiscard]] bool supports_type_conversion_from(ast::builtin_type from_type, token_type op) const override;

	void execute_binary_operation(eval_value& result, const ast::expression_value& from, const builtin_type_symbol& right_val, token_type op, type_operation_context& type_operation_context) const override;
	
public:
	explicit builtin_boolean_impl(builtin_type_symbol* builtin_type_symbol);

	virtual ~builtin_boolean_impl() = default;
	builtin_boolean_impl(const builtin_boolean_impl& other) = delete;
	builtin_boolean_impl(builtin_boolean_impl&& other) noexcept = delete;
	builtin_boolean_impl& operator=(const builtin_boolean_impl& other) = delete;
	builtin_boolean_impl& operator=(builtin_boolean_impl&& other) noexcept = delete;
};

class builtin_integer_impl final : public builtin_type_impl
{
protected:
	[[nodiscard]] bool supports_type_conversion_from(ast::builtin_type from_type, token_type op) const override;

	void execute_binary_operation(eval_value& result, const ast::expression_value& from, const builtin_type_symbol& right_val, token_type op, type_operation_context& type_operation_context) const override;
	
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
	[[nodiscard]] bool supports_type_conversion_from(ast::builtin_type from_type, token_type op) const override;
	
	void execute_binary_operation(eval_value& current, const ast::expression_value& from, const builtin_type_symbol& right_val, token_type op, type_operation_context& type_operation_context) const override;
	
public:
	explicit builtin_string_impl(builtin_type_symbol* builtin_type_symbol);

	virtual ~builtin_string_impl() = default;
	builtin_string_impl(const builtin_string_impl& other) = delete;
	builtin_string_impl(builtin_string_impl&& other) noexcept = delete;
	builtin_string_impl& operator=(const builtin_string_impl& other) = delete;
	builtin_string_impl& operator=(builtin_string_impl&& other) noexcept = delete;
};