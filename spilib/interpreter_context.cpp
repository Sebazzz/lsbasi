#include "pch.h"
#include "interpreter_context.h"

builtin_string_ptr string_pool::get_or_add(const builtin_string& str)
{
	const auto result = this->m_pool.insert(str);

	// This looks weird but is a valid way to get a string from the pool. This pointer remains valid as
	// long the pool is kept alive. The pool is valid as long as the interpreter context is kept alive.
	return &*result.first;
}
