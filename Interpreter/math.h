#pragma once
#include <limits>

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