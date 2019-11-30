#pragma once
#include <cstdint>

/**
 * Interpretation built-in type: a INTEGER is a 32-bit integer
 */
using builtin_integer = int32_t;
static_assert(sizeof(builtin_integer) == 32/8, L"Expect built-in integers to be 32-bit ");

/**
 * Interpretation built-in type: a REAL is a 64-bits double
 */
using builtin_real = double;
static_assert(sizeof(builtin_real) == 64/8, L"Expect built-in real to be 64-bit");
