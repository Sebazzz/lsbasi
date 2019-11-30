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
	void visit(ast::block& block) override;
	void visit(ast::program& program) override;
	
	/**
	 * Gets the global scope. This is primarily for unit testing purposes where we want to check if
	 * the interpreter did its job.
	 */
	[[nodiscard]] std::shared_ptr<scope_context> global_scope() const;
};

