#pragma once
#include "ast_node_visitor.h"
#include "var.h"
#include "eval_visitor.h"
#include "scope_context.h"

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

