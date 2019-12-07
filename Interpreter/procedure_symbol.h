#pragma once
#include "symbol.h"
#include "routine.h"
#include "var_decl.h"

struct scope_context;

/**
 * Represents a procedure
 */
class procedure_symbol abstract : public symbol
{
public:
	explicit procedure_symbol(const symbol_identifier& identifier);
	~procedure_symbol() override = default;
	
	[[nodiscard]] std::wstring to_string() const override;

	[[nodiscard]] virtual symbol_type_ptr<type_symbol> return_type() const = 0;
	[[nodiscard]] virtual bool is_function() const = 0;
	[[nodiscard]] virtual const ast::procedure_param_list& params() const = 0;
	[[nodiscard]] virtual symbol_table& symbol_table() const = 0;
};

/**
 * Represents a user-defined procedure
 */
class user_defined_procedure_symbol final : public procedure_symbol
{
private:
	ast::routine& m_procedure;
	
public:
	explicit user_defined_procedure_symbol(ast::routine& procedure);
	
	[[nodiscard]] const ast::routine& procedure() const;

	[[nodiscard]] symbol_type_ptr<type_symbol> return_type() const override;
	[[nodiscard]] bool is_function() const override;
	[[nodiscard]] const ast::procedure_param_list& params() const override;
	[[nodiscard]] ::symbol_table& symbol_table() const override;
};

/**
 * Represents a built-in procedure. Derived classes actually implement the procedure logic.
 */
class builtin_procedure_symbol abstract : public procedure_symbol
{
	private:
		/**
		 * Lazily initialized - does not change class behavior
		 */
		mutable std::unique_ptr<::symbol_table> m_symbol_table;

		::symbol_table* m_runtime_symbol_table;
	
	protected:
		ast::procedure_param_list m_param_list;
	
		builtin_procedure_symbol(const symbol_identifier& identifier, ::symbol_table* runtime_symbol_table);
		ast::type_ptr get_builtin_type(ast::builtin_type type_spec) const;
		void register_param(const symbol_identifier& identifier, ast::builtin_type builtin_type);
		

	public:
		[[nodiscard]] const ast::procedure_param_list& params() const override;
		[[nodiscard]] ::symbol_table& symbol_table() const override;
		virtual void invoke(scope_context& procedure_scope) = 0;
};
