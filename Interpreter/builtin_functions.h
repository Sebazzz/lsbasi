#pragma once
#include "builtin_type_traits.h"
#include "memory_table.h"
#include "scope_context.h"
#include "routine_symbol.h"
#include "builtin_procedures.h"

/**
 * Base template for built-in procedures. Most of the boilerplate is handled by this template.
 * The only thing the derived implementation needs to do is overriding the invoke_impl method.
 *
 * The template parameters are the types of arguments provided to the built-in procedure.
 * The first parameter is the return value.
 */
template<ast::builtin_type TReturn, ast::builtin_type... Ts>
class builtin_function abstract : public builtin_routine_symbol
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
	builtin_function(const symbol_identifier& identifier, ::symbol_table* runtime_symbol_table): builtin_routine_symbol(identifier, runtime_symbol_table)
	{
		// See the comment in invoke(...)
#pragma warning( push )
#pragma warning( disable : 4189 )
		auto param_id = 0;
		
		((void) this->register_param(builtin_type_traits<Ts>::param_id(++param_id), Ts), ...);
#pragma warning( pop )
	}

	/**
	 * The actual implementation of the built-in function
	 */
	virtual typename builtin_type_traits<TReturn>::builtin_type invoke_impl(typename builtin_type_traits<Ts>::builtin_type...) = 0;

public:
	void invoke(scope_context& procedure_scope) override
	{
		// We need to disable CS4189 regarding unused symbol: When a function accepts no
		// parameters the param_id variable is defined, but no calls to make_arg are made,
		// so the param_id parameter is never used.
#pragma warning( push )
#pragma warning( disable : 4189 )
		auto param_id = 0;
		auto ret_val = this->invoke_impl(make_arg<Ts>(procedure_scope, ++param_id)...);
		this->set_result(procedure_scope, builtin_type_traits<TReturn>::make_expression_value(ret_val));
#pragma warning( pop )
	}

	[[nodiscard]] bool is_function() const override
	{
		return true;
	}

	[[nodiscard]] symbol_type_ptr<type_symbol> return_type() const override
	{
		return this->get_builtin_type_symbol(TReturn);
	}
};

/**
 * Represents a builtin-function: random
 */
class builtin_function_random final : public builtin_function<ast::builtin_type::real>
{
protected:
	builtin_type_traits<ast::builtin_type::real>::builtin_type invoke_impl() override;
	
public:
	builtin_function_random(::symbol_table* runtime_symbol_table);
};
