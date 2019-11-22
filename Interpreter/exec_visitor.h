#pragma once
#include "ast_node_visitor.h"
class exec_visitor :
	public ast_node_visitor
{
private:

public:
	~exec_visitor() override = default;
	exec_visitor();
	exec_visitor(const exec_visitor& other) = delete;
	exec_visitor(exec_visitor&& other) noexcept = delete;
	exec_visitor& operator=(const exec_visitor& other) = delete;
	exec_visitor& operator=(exec_visitor&& other) noexcept = delete;

	void visit(ast::ast_node& node) override;
};

