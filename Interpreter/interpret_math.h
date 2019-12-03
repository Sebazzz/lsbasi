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
        throw exec_error("Integer overflow - " + std::to_string(result), {-1,-1});
    }

	if (result < 0 && operand < std::numeric_limits<T>::min() - result) {
		throw exec_error("Integer underflow -" +  std::to_string(result), {-1,-1});
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
        throw exec_error("Floating point overflow - " + std::to_string(result), {-1,-1});
	}

	if (std::fetestexcept(FE_UNDERFLOW)) {
        throw exec_error("Floating point underflow - " + std::to_string(result), {-1,-1});
	}
}

template <typename T>
void subtract_interpret(T& result, const T& operand)
{
	add_interpret(result, -1 * operand);
}

template <typename T>
void divide_interpret(T& result, const T& operand)
{
	if (operand == 0)
	{
		throw exec_error("Divide by zero - " + std::to_string(result), {-1,-1});
	}

	result /= operand;
}

template <typename T>
void multiply_interpret(T& result, const T& operand)
{
	// TODO: implement overflow detection
	result *= operand;
}