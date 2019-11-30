#pragma once
#include "symbol.h"

/**
 * Abstract class that represents a type (either user-defined or built-in)
 */
class type_symbol abstract : public symbol
{

public:
	type_symbol(const symbol_identifier& identifier, symbol_type symbol = symbol_type::type);

	/**
	 * Gets the type implementation for this symbol
	 */
	virtual const type_impl& type_impl() = 0;
};