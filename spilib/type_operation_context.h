#pragma once
#include "interpreter_context.h"
#include "symbol_table.h"

/**
 * Helper struct which provides additional contextual information about an operation on a type
 *
 * This is primarily done because refactoring is hard, and we need more parameters in the type
 * implementation than you'd general see as acceptable
 */
struct type_operation_context
{
	interpreter_context& interpreter_context;
	symbol_table& symbol_table;
	line_info line_info;
};