#pragma once
#include "ast_node_visitor.h"

/**
 * Reverse-polish notation visitor
 */
class lisp_notation_visitor :	public ast_node_visitor
{
	private:
	std::wstring string_buf;

	static wchar_t op_to_string(token_type token);
	
public:
	~lisp_notation_visitor() override = default;
	lisp_notation_visitor() = default;
	lisp_notation_visitor(const lisp_notation_visitor& other) = delete;
	lisp_notation_visitor(lisp_notation_visitor&& other) noexcept = delete;
	lisp_notation_visitor& operator=(const lisp_notation_visitor& other) = delete;
	lisp_notation_visitor& operator=(lisp_notation_visitor&& other) noexcept = delete;
	
	void visit(bin_op& binaryOperator) override;
	void visit(num& number) override;
	void visit(ast_node& node) override;
	void visit(unary_op& unaryOperator) override;
	
	std::wstring& get_string();
};

