#pragma once
#include "pch.h"
#include "ast_node_visitor.h"
#include "ast_common.h"

class eval_visitor : public ast_node_visitor
{
public:
	struct eval_value
	{
		ast::var_type type;
		ast::expression_value value;
	};
	
private:
	eval_value m_result;

	/*
	 * Hold the last evaluated items. This to work around that we don't have visitor with return type.
	 */
	std::stack<eval_value> m_stack;

protected:
	/**
	 * Helper function to visit the specified node and get the result back
	 */
	template <class T>
	eval_value accept(T& node);

	/**
	 * Helper method to register the result for further up the call stack
	 */
	void register_visit_result(eval_value result);

public:
	~eval_visitor() override = default;
	eval_visitor();
	eval_visitor(const eval_visitor& other) = delete;
	eval_visitor(eval_visitor&& other) noexcept = delete;
	eval_visitor& operator=(const eval_visitor& other) = delete;
	eval_visitor& operator=(eval_visitor&& other) noexcept = delete;
	
	void visit(ast::bin_op& binaryOperator) override;
	void visit(ast::num& number) override;
	void visit(ast::ast_node& node) override;
	void visit(ast::unary_op& unaryOperator) override;
	
	[[nodiscard]] eval_value result() const;
};

template <class T>
eval_visitor::eval_value eval_visitor::accept(T& node)
{
	node.accept(*this);

	if (this->m_stack.empty())
	{
		throw interpret_except("Previous node accept did not yield a value", typeid(node).name());
	}

	const eval_value val = this->m_stack.top();
	this->m_stack.pop();
	return val;
}

