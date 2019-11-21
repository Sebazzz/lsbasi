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
	
	void visit(bin_op& binaryOperator) override;
	void visit(num& number) override;
	void visit(ast_node& node) override;
	void visit(unary_op& unaryOperator) override;
	
	std::wstring& get_string();
};

