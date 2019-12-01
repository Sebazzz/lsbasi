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
	 * Assign a evaluation value (eval_value) to the current type
	 */
    virtual void convert_value(eval_value& eval_value, line_info line_info) const = 0;

	/**
	 * Widens the evaluation value (eval_value) to the current type
	 */
    virtual void change_type(eval_value& eval_value, line_info line_info) const = 0;
};


