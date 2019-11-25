#pragma once
#include "ast_node_visitor.h"
#include "var.h"
#include <map>
#include <stack>
#include "eval_visitor.h"
#include "util.h"

struct stack_context
{
	std::map<ast::var_identifier, double, case_insensitive_string_comparer> variables;
};

class stack
{
private:
	std::stack<stack_context> m_stack;

public:
	stack_context& current_context();
	stack_context& push();
	void pop();
};

class exec_visitor :
	public eval_visitor
{
private:
	stack m_stack;
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
	
	[[nodiscard]] std::wstring last_value() const;
};

