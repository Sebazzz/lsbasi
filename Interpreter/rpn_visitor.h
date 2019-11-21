#pragma once
#include "ast_node_visitor.h"

/**
 * Reverse-polish notation visitor
 */
class rpn_visitor :	public ast_node_visitor
{
	private:
	std::wstring string_buf;

	static wchar_t op_to_string(token_type token);
	
public:
	~rpn_visitor() override = default;
	rpn_visitor() = default;
	rpn_visitor(const rpn_visitor& other) = delete;
	rpn_visitor(rpn_visitor&& other) noexcept = delete;
	rpn_visitor& operator=(const rpn_visitor& other) = delete;
	rpn_visitor& operator=(rpn_visitor&& other) noexcept = delete;
	
	void visit(ast::bin_op& binaryOperator) override;
	void visit(ast::num& number) override;
	void visit(ast::ast_node& node) override;
	void visit(ast::unary_op& unaryOperator) override;
	
	std::wstring& get_string();
};

