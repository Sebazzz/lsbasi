#pragma once
#include "eval_value.h"

class symbol;

/**
 * Interface for type implementations. The current type implementation knows
 * what symbol it belongs to.
 */
class type_impl abstract
{
public:
    virtual ~type_impl() = default;

    type_impl() = default;
    type_impl(const type_impl& other) = delete;
    type_impl(type_impl&& other) noexcept = delete;
    type_impl& operator=(const type_impl& other) = delete;
    type_impl& operator=(type_impl&& other) noexcept = delete;

	/**
	 * Executes a binary operation on the the right value and the result (being the current type)
	 */
    virtual void execute_binary_operation(eval_value& result, const eval_value& right_val, token_type op, line_info line_info) const = 0;

	/**
	 * Execute implicit type conversion
	 */
	virtual void implicit_type_conversion(eval_value& value) const = 0;

	/**
	 * Gets if the current type supports conversion from the specific type
	 */
	virtual bool supports_implicit_type_conversion_from(symbol_type_ptr<type_symbol> type, token_type op) const = 0;
};


