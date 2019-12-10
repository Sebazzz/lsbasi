#include "pch.h"
#include "type_symbol.h"

type_symbol::type_symbol(const symbol_identifier& identifier, symbol_type symbol): symbol(symbol, identifier)
{
}
