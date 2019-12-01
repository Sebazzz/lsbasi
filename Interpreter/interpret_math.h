#pragma once
#include <limits>
#include <cfenv>

#include "interpret_except.h"
#include "memory_table.h"
#include "builtin_type_symbol.h"

template <typename T>
void add_interpret(T& result, const T& operand)
{
	if (result > 0 && operand > std::numeric_limits<T>::max() - result) {
        throw interpret_except("Integer overflow - " + std::to_string(result));
    }

	if (result < 0 && operand < std::numeric_limits<T>::min() - result) {
		throw interpret_except("Integer underflow -" +  std::to_string(result));
	}

	result = result + operand;
}

template<>
inline void add_interpret(builtin_real& result, const builtin_real& operand)
{
	std::feclearexcept(FE_OVERFLOW);
    std::feclearexcept(FE_UNDERFLOW);

	result = result + operand;

	if (std::fetestexcept(FE_OVERFLOW)) {
        throw interpret_except("Floating point overflow - " + std::to_string(result));
	}

	if (std::fetestexcept(FE_UNDERFLOW)) {
        throw interpret_except("Floating point underflow - " + std::to_string(result));
	}
}

template <>
inline void add_interpret(eval_value& result, const eval_value& operand)
{
	const auto symbol_type = dynamic_cast<builtin_type_symbol&>(*result.type.get()).type();

	switch (symbol_type)
	{
	case ast::builtin_type::integer:
		add_interpret(result.value.int_val, operand.value.int_val);
		break;
	case ast::builtin_type::real:
		add_interpret(result.value.real_val, operand.value.real_val);
		break;
	default: ;
	}
}

template <typename T>
void subtract_interpret(T& result, const T& operand)
{
	add_interpret(result, -1 * operand);
}

template <>
inline void subtract_interpret(eval_value& result, const eval_value& operand)
{
	const auto symbol_type = dynamic_cast<builtin_type_symbol&>(*result.type.get()).type();

	switch (symbol_type)
	{
	case ast::builtin_type::integer:
		subtract_interpret(result.value.int_val, operand.value.int_val);
		break;
	case ast::builtin_type::real:
		subtract_interpret(result.value.real_val, operand.value.real_val);
		break;
	default: ;
	}
}

template <typename T>
void divide_interpret(T& result, const T& operand)
{
	if (operand == 0)
	{
		throw interpret_except("Divide by zero - " + std::to_string(result));
	}

	result /= operand;
}

template <>
inline void divide_interpret(eval_value& result, const eval_value& operand)
{
	const auto symbol_type = dynamic_cast<builtin_type_symbol&>(*result.type.get()).type();

	switch (symbol_type)
	{
	case ast::builtin_type::integer:
		divide_interpret(result.value.int_val, operand.value.int_val);
		break;
	case ast::builtin_type::real:
		divide_interpret(result.value.real_val, operand.value.real_val);
		break;
	default: ;
	}
}

template <typename T>
void multiply_interpret(T& result, const T& operand)
{
	// TODO: implement overflow detection
	result *= operand;
}

template <>
inline void multiply_interpret(eval_value& result, const eval_value& operand)
{
	const auto symbol_type = dynamic_cast<builtin_type_symbol&>(*result.type.get()).type();

	switch (symbol_type)
	{
	case ast::builtin_type::integer:
		multiply_interpret(result.value.int_val, operand.value.int_val);
		break;
	case ast::builtin_type::real:
		multiply_interpret(result.value.real_val, operand.value.real_val);
		break;
	default: ;
	}
}

inline void negate_interpret(eval_value& result)
{
	const auto symbol_type = dynamic_cast<builtin_type_symbol&>(*result.type.get()).type();

	switch (symbol_type)
	{
	case ast::builtin_type::integer:
		result.value.int_val *= -1;
		break;
	case ast::builtin_type::real:
		result.value.real_val *= -1;
		break;
	default: ;
	}
}