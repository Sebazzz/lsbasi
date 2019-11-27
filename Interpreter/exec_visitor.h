#pragma once
#include "ast_node_visitor.h"
#include "var.h"
#include <map>
#include <stack>
#include "eval_visitor.h"
#include "util.h"

class variable_register
{
private:
	struct var
	{
		double value;
		ast::var_type type;
		bool is_from_parent_scope;
	};
	
	std::map<ast::var_identifier, var, case_insensitive_string_comparer> m_variables;

	void set_from_parent(const ast::var_identifier& identifier, const var& var_info);

	/**
	 * Contains a pointer to the parent scope. I believe raw pointer usage is justified because
	 * we always either have a previous or we don't but the previous can never be dangling.
	 */
	variable_register* m_previous = nullptr;

public:
	double get(const ast::var_identifier& identifier);

	void declare(const ast::var_identifier& identifier, ast::var_type type);

	void set(const ast::var_identifier& identifier, double value);

	static std::unique_ptr<variable_register> create_from_parent_scope(const variable_register* parent);
	
	void copy_to_parent();
};

struct scope_context
{
	std::unique_ptr<variable_register> variables;
};

class scope_manager
{
private:
	std::stack<scope_context> m_scope;

public:
	/**
	 * Gets the current context. Throws if not available.
	 */
	scope_context& current_context();

	/**
	 * 
	 */
	scope_context& push();

	/**
	 * Reverts to the previous scope
	 */
	void pop();
};

class exec_visitor :
	public eval_visitor
{
private:
	scope_manager m_stack;
	std::wstring m_last_value;
	
public:
	~exec_visitor() override = default;
	exec_visitor();
	exec_visitor(const exec_visitor& other) = delete;
	exec_visitor(exec_visitor&& other) noexcept = delete;
	exec_visitor& operator=(const exec_visitor& other) = delete;
	exec_visitor& operator=(exec_visitor&& other) noexcept = delete;

	void visit(ast::ast_node& node) override;
	void visit(ast::compound& compound) override;
	void visit(ast::assign& assign) override;
	void visit(ast::var& var) override;
	void visit(ast::var_decl& var_decl) override;
	void visit(ast::block& block) override;
	
	[[nodiscard]] std::wstring last_value() const;
};

