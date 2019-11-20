#pragma once
#include <limits>
#include <cfenv>

#include "interpret_except.h"

template <typename T>
void add_interpret(T& result, T operand)
{
	if (result > 0 && operand > std::numeric_limits<T>::max() - result) {
        throw interpret_except("Integer overflow");
    }

	if (result < 0 && operand < std::numeric_limits<T>::min() - result) {
		throw interpret_except("Integer underflow");
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
        throw interpret_except("Floating point overflow");
	}

	if (std::fetestexcept(FE_UNDERFLOW)) {
        throw interpret_except("Floating point underflow");
	}
}

template <typename T>
void subtract_interpret(T& result, T operand)
{
	add_interpret(result, -1 * operand);
}

template <typename T>
void divide_interpret(T& result, T operand)
{
	if (operand == 0)
	{
		throw interpret_except("Divide by zero");
	}

	result /= operand;
}