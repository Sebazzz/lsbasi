#pragma once
#include "symbol.h"

class type_symbol;

struct eval_value {
	symbol_type_ptr<type_symbol> type;
	ast::expression_value value;
};