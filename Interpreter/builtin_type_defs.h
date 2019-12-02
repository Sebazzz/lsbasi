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

/**
 * Interpretation built-in type: a STRING is a wide-character string
 */
using builtin_string = std::wstring;

/**
 * For performance reasons we use a global string table and just use a pointer in the expression values.
 * We can't use a std::wstring because it is 28 bytes which would inflate the struct by 20 bytes or more, depending on the arch.
 * We can't use a std::shared_ptr because the union then needs much custom copy logic which I'm currently not willing to provide
 * and would still inflate the data type with one additional pointer (see: https://stackoverflow.com/q/30492927/646215)
 */
using builtin_string_ptr = const builtin_string*;
static_assert(sizeof(builtin_string_ptr) == sizeof(void*), L"Expect built-in string pointer to be of one pointer");
