#pragma once

class symbol_table
{
private:
	struct symbol_info
	{
		double value;
		ast::var_type type;
		bool is_from_parent_scope;
	};
	
	std::map<ast::var_identifier, symbol_info, case_insensitive_string_comparer> m_variables;

	void set_from_parent(const ast::var_identifier& identifier, const symbol_info& var_info);

	/**
	 * Contains a pointer to the parent scope. I believe raw pointer usage is justified because
	 * we always either have a previous or we don't but the previous can never be dangling.
	 */
	symbol_table* m_previous = nullptr;

public:
	double get(const ast::var_identifier& identifier);

	void declare(const ast::var_identifier& identifier, ast::var_type type);

	void set(const ast::var_identifier& identifier, double value);

	static std::unique_ptr<symbol_table> create_from_parent_scope(const symbol_table* parent);
	
	void copy_to_parent();
};