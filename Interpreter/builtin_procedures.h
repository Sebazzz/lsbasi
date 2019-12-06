#pragma once
#include "builtin_type_traits.h"
#include "memory_table.h"
#include "scope_context.h"
#include "procedure_symbol.h"
#include "builtin_procedures.h"

/**
 * Base template for built-in procedures. Most of the boilerplate is handled by this template.
 * The only thing the derived implementation needs to do is overriding the invoke_impl method.
 *
 * The template parameter is the type of argument provided to the built-in string.
 */
template<ast::builtin_type T>
class builtin_procedure abstract : public builtin_procedure_symbol
{
	using arg_traits = builtin_type_traits<T>;
	
protected:
	builtin_procedure(const symbol_identifier& identifier, ::symbol_table* runtime_symbol_table): builtin_procedure_symbol(identifier, runtime_symbol_table)
	{
		this->register_param(arg_traits::param_id(), T);
	}

	/**
	 * The actual implementation of the built-in function
	 */
	virtual void invoke_impl(typename arg_traits::builtin_type arg) = 0;

public:
	void invoke(scope_context& procedure_scope) override
	{
		auto raw_arg = procedure_scope.memory->get(procedure_scope.symbols.get(arg_traits::param_id()));
		this->invoke_impl(arg_traits::get_from_expression(raw_arg));
	}
};

/**
 * Represents the builtin-procedure: writeln
 */
class builtin_procedure_writeln final : public builtin_procedure<ast::builtin_type::string>
{
protected:
	void invoke_impl(arg_traits::builtin_type arg) override;
	
public:
	builtin_procedure_writeln(::symbol_table* runtime_symbol_table);
};
