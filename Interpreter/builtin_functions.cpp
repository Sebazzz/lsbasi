#include "pch.h"
#include "builtin_functions.h"

#include <random>

builtin_type_traits<ast::builtin_type::real>::builtin_type builtin_function_random::invoke_impl()
{
	static std::uniform_real_distribution<builtin_real> unif(-1000,1000);
	static std::default_random_engine random_impl;
	return unif(random_impl);
}

builtin_function_random::builtin_function_random(::symbol_table* runtime_symbol_table): builtin_function(L"random", runtime_symbol_table)
{
}
