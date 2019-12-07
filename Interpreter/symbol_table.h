#pragma once
#include "pch.h"
#include "symbol.h"

class builtin_procedure_symbol;
class procedure_symbol;
class type_symbol;

class symbol_table final
{
private:
	std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer> m_variables;

	/**
	 * Contains a pointer to the parent symbol table. I believe raw pointer usage is justified because
	 * we always either have a parent scope (thus memory) or we don't but the previous scope can never be dangling.
	 */
	symbol_table* m_parent = nullptr;

	symbol_type_ptr<procedure_symbol> m_associated_procedure;

	std::wstring m_scope_name;

	/**
	 * This private constructor creates a global scope
	 */
	explicit symbol_table();

	void register_builtin_type(ast::builtin_type type_spec);

	template<class T>
	void register_builtin_procedure();

public:
	explicit symbol_table(std::wstring scope_name, symbol_table* parent = nullptr);

	class symbol_table_iterator
	{
		friend class symbol_table;

		std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator iterator;
		const std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator end;

		explicit symbol_table_iterator(
			std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator iterator,
			const std::map<symbol_identifier, symbol_ptr, case_insensitive_string_comparer>::const_iterator end);

	public:
		symbol_ptr operator->() const;

		[[nodiscard]] symbol_ptr get() const;

		[[nodiscard]] bool is_at_end() const;

		void next();
	};
	
	/**
	 * Gets a symbol from the current or parent scope. Throws if not found.
	 */
	symbol_ptr get(const symbol_identifier& identifier);

	/**
	 * Gets a built-in symbol
	 */
	symbol_type_ptr<type_symbol> get(const ast::builtin_type type_spec);

	/**
	 * Gets a symbol from the current or parent scope. Throws if not found or not of the expected type.
	 */
	template <class T>
	symbol_type_ptr<T> get(const symbol_identifier& identifier);

	/**
	 * Declares a symbol. Throws if already declared.
	 */
	void declare(const symbol_ptr& symbol, line_info position);

	/**
	 * Gets an iterator at allows iterating over all the symbols
	 */
	[[nodiscard]] symbol_table_iterator iterator() const;

	/**
	 * Sets the current symbol table associated procedure it belongs to so the memory table can at a later stage use this information to pre-define variables.
	 */
	void associated_routine(const symbol_type_ptr<procedure_symbol>& routine);

	/**
	 * Gets, if available, the routine associated with this symbol
	 */
	[[nodiscard]] symbol_type_ptr<procedure_symbol> associated_routine() const;
};

template <class T>
void symbol_table::register_builtin_procedure()
{
	const symbol_type_ptr<T> procedure = make_symbol_ptr<T>(this);

	this->m_variables.try_emplace(
		procedure->identifier(),
		procedure
	);
}

template <class T>
symbol_type_ptr<T> symbol_table::get(const symbol_identifier& identifier)
{
	static_assert(std::is_convertible<T*, symbol*>::value, "This template is only suitable for symbols");

	// Get and cast
	const symbol_ptr ptr = this->get(identifier);
	const auto result = std::dynamic_pointer_cast<T, symbol>(ptr);

	if (ptr && !result)
	{
		// Note: GCC does not give very good type information. MSVC does, and that what I use.
		// If this should give better error messages on GCC some more elaborate tricks are needed
		// See also: 
		throw exec_error(L"Expected symbol '" + ptr->to_string() + L"' to be of a different type: " + string_to_wstring(typeid(T).name()), {-1,-1});
	}

	return result;
}
