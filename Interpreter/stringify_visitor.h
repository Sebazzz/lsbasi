#pragma once
#include "ast_node_visitor.h"

class stringify_visitor final : public ast_node_visitor
{
private:
	std::wstring string_buf;
	
public:
	~stringify_visitor() override = default;
	stringify_visitor() = default;
	stringify_visitor(const stringify_visitor& other) = delete;
	stringify_visitor(stringify_visitor&& other) noexcept = delete;
	stringify_visitor& operator=(const stringify_visitor& other) = delete;
	stringify_visitor& operator=(stringify_visitor&& other) noexcept = delete;
	
	void visit(ast::bin_op& binaryOperator) override;
	void visit(ast::num& number) override;
	void visit(ast::ast_node& node) override;
	void visit(ast::unary_op& unaryOperator) override;

	void visit(ast::compound& compound) override;
	void visit(ast::assign& assign) override;
	void visit(ast::var& variable) override;
	void visit(ast::empty& empty_statement) override;
	
	std::wstring& get_string();
};

