#pragma once
#include <limits>
#include <cfenv>

#include "interpret_except.h"
#include "symbol_table.h"

template <typename T>
void add_interpret(T& result, T operand)
{
	if (result > 0 && operand > std::numeric_limits<T>::max() - result) {
        throw interpret_except("Integer overflow", std::to_string(result));
    }

	if (result < 0 && operand < std::numeric_limits<T>::min() - result) {
		throw interpret_except("Integer underflow",  std::to_string(result));
	}

	result = result + operand;
}

template<>
inline void add_interpret(double& result, double operand)
{
	std::feclearexcept(FE_OVERFLOW);
    std::feclearexcept(FE_UNDERFLOW);

	result = result + operand;

	if (std::fetestexcept(FE_OVERFLOW)) {
        throw interpret_except("Floating point overflow", std::to_string(result));
	}

	if (std::fetestexcept(FE_UNDERFLOW)) {
        throw interpret_except("Floating point underflow", std::to_string(result));
	}
}

template <>
inline void add_interpret(symbol_contents& result, const symbol_contents operand)
{
	const auto symbol_type = result.type;

	switch (symbol_type)
	{
	case ast::var_type::integer:
		add_interpret(result.value.int_val, operand.value.int_val);
		break;
	case ast::var_type::real:
		add_interpret(result.value.real_val, operand.value.real_val);
		break;
	default: ;
	}
}

template <typename T>
void subtract_interpret(T& result, T operand)
{
	add_interpret(result, -1 * operand);
}

template <>
inline void subtract_interpret(symbol_contents& result, const symbol_contents operand)
{
	const auto symbol_type = result.type;

	switch (symbol_type)
	{
	case ast::var_type::integer:
		subtract_interpret(result.value.int_val, operand.value.int_val);
		break;
	case ast::var_type::real:
		subtract_interpret(result.value.real_val, operand.value.real_val);
		break;
	default: ;
	}
}

template <typename T>
void divide_interpret(T& result, T operand)
{
	if (operand == 0)
	{
		throw interpret_except("Divide by zero",  std::to_string(result));
	}

	result /= operand;
}

template <>
inline void divide_interpret(symbol_contents& result, const symbol_contents operand)
{
	const auto symbol_type = result.type;

	switch (symbol_type)
	{
	case ast::var_type::integer:
		divide_interpret(result.value.int_val, operand.value.int_val);
		break;
	case ast::var_type::real:
		divide_interpret(result.value.real_val, operand.value.real_val);
		break;
	default: ;
	}
}

template <typename T>
void multiply_interpret(T& result, T operand)
{
	// TODO: implement overflow detection
	result *= operand;
}

template <>
inline void multiply_interpret(symbol_contents& result, const symbol_contents operand)
{
	const auto symbol_type = result.type;

	switch (symbol_type)
	{
	case ast::var_type::integer:
		multiply_interpret(result.value.int_val, operand.value.int_val);
		break;
	case ast::var_type::real:
		multiply_interpret(result.value.real_val, operand.value.real_val);
		break;
	default: ;
	}
}