#pragma once
#include "symbol.h"

class type_symbol;

/**
 * Contains the value of an evaluation with type information
 *
 * Note: this type has debugging support in execution.natvis
 */
struct eval_value {
	symbol_type_ptr<type_symbol> type;
	ast::expression_value value;
};

static_assert(sizeof(eval_value) == (sizeof(ast::expression_value) + sizeof(void*) + sizeof(void*)), 
 R"(Expect data size to be of 64-bit + two pointers:
- this is 128 bits (16 bytes) on x86 
- this is 192 bits (24 bytes) on x64

The shared_ptr to the symbol contains two pointers, so either 8 or 16 bytes. 
Might want consider using a raw pointer for that which would shrink this struct
size by one pointer size to 12 or 16 bytes. Or one alternative is to pass this 
struct by reference of pointer. I need to benchmark that, and perhaps with the 
large caches of my CPU (Ryzen) I might never see a difference.
)");
