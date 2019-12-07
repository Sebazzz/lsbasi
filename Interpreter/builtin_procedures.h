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
 * The template parameters are the types of arguments provided to the built-in procedure.
 * Example usage:
 *
 * class builtin_procedure_add final : public builtin_procedure<ast::builtin_type::integer, ast::builtin_type::real> {
 *    protected:
 *	     void invoke_impl(builtin_type_traits<ast::builtin_type::integer>::builtin_type, builtin_type_traits<ast::builtin_type::real>::builtin_type) override;
 * }
 */
template<ast::builtin_type... Ts>
class builtin_procedure abstract : public builtin_procedure_symbol
{
private:
	/**
	 * Helper function for getting the argument value for a given T
	 */
	template<ast::builtin_type T>
	typename builtin_type_traits<T>::builtin_type make_arg(scope_context& procedure_scope, int param_id)
	{
		auto raw_arg = procedure_scope.memory->get(procedure_scope.symbols.get(builtin_type_traits<T>::param_id(param_id++)));
		return builtin_type_traits<T>::get_from_expression(raw_arg);
	}
	
protected:
	builtin_procedure(const symbol_identifier& identifier, ::symbol_table* runtime_symbol_table): builtin_procedure_symbol(identifier, runtime_symbol_table)
	{
		auto param_id = 0;
		
		((void) this->register_param(builtin_type_traits<Ts>::param_id(++param_id), Ts), ...);
	}

	/**
	 * The actual implementation of the built-in function
	 */
	virtual void invoke_impl(typename builtin_type_traits<Ts>::builtin_type...) = 0;

public:
	void invoke(scope_context& procedure_scope) override
	{
		auto param_id = 0;
		this->invoke_impl(make_arg<Ts>(procedure_scope, ++param_id)...);
	}
};

/**
 * Represents the builtin-procedure: writeln
 */
class builtin_procedure_writeln final : public builtin_procedure<ast::builtin_type::string>
{
protected:
	void invoke_impl(builtin_type_traits<ast::builtin_type::string>::builtin_type) override;

public:
	builtin_procedure_writeln(::symbol_table* runtime_symbol_table);
};
