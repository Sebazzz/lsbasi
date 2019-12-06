#include "pch.h"
#include <iostream>
#include "builtin_procedures.h"

// ----- writeln(STRING)

void builtin_procedure_writeln::invoke_impl(arg_traits::builtin_type arg)
{
	if (!arg)
	{
		// Uninitialized write just yields an empty line
		std::wcout << std::endl;
	} else
	{
		// Dereference for write. This is safe because the pointer points to the string pool, never to other sources.
		std::wcout << *arg << std::endl;
	}
}

builtin_procedure_writeln::builtin_procedure_writeln(::symbol_table* runtime_symbol_table): builtin_procedure(
	L"writeln", runtime_symbol_table)
{
}
