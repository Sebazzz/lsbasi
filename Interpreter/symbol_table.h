#pragma once

struct symbol_contents
{
	ast::symbol_value value;
	ast::var_type type;

	[[nodiscard]] std::wstring to_string() const
	{
		std::wstring output;
		switch (this->type)
		{
		case ast::var_type::integer:
			output += L"integer [";
			output += std::to_wstring(this->value.int_val);
			output += L"]";
			break;
		case ast::var_type::real:
			output += L"real - [";
			output += std::to_wstring(this->value.int_val);
			output += L"]";
			break;
		default:
			output = L"unknown"
			;
		}

		return output;
	}
};

class symbol_table
{
private:
	struct symbol_info
	{
		symbol_contents symbol_contents;
		bool is_from_parent_scope;
	};
	
	std::map<ast::var_identifier, symbol_info, case_insensitive_string_comparer> m_variables;

	void set_from_parent(const ast::var_identifier& identifier, const symbol_info& var_info);

	/**
	 * Contains a pointer to the parent scope. I believe raw pointer usage is justified because
	 * we always either have a previous scope (thus symbols) or we don't but the previous scope can never be dangling.
	 */
	symbol_table* m_previous = nullptr;

public:
	symbol_contents get(const ast::var_identifier& identifier);

	void declare(const ast::var_identifier& identifier, ast::var_type type);

	void ensure_type(const ast::var_identifier& identifier, ast::var_type type);
	
	void set(const ast::var_identifier& identifier, ast::symbol_value value);

	static std::unique_ptr<symbol_table> create_from_parent_scope(const symbol_table* parent);
	
	void copy_to_parent();
};