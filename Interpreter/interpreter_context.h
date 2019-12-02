#pragma once
#include <set>
#include "builtin_type_defs.h"

/**
 * Represents a string pool used for optimizing string allocations.
 *
 * Currently we never remove allocated string from the pool. Strings are immutable as far as the pool is concerned.
 */
class string_pool
{
private:
	std::set<builtin_string> m_pool;

public:
	builtin_string_ptr get_or_add(const builtin_string& str);
};

/**
 * General context for the interpreter. A kind-of global bookkeeping.
 * In later stages this might also be the interface to the "world" (stdin/stdout)
 */
struct interpreter_context
{
	string_pool string_pool;
};

using interpreter_context_ptr = std::shared_ptr<interpreter_context>;