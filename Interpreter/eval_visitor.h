#pragma once
#include "ast_node_visitor.h"
#include <stack>
#include "interpret_except.h"

class eval_visitor final : public ast_node_visitor
{
private:
	double m_result;

	/*
	 * Hold the last evaluated items. This to work around that we don't have visitor with return type.
	 */
	std::stack<double> m_stack;

	/**
	 * Helper function to visit the specified node and get the result back
	 */
	template <class T>
	double accept(T& node);

	/**
	 * Helper method to register the result for further up the call stack
	 */
	void register_visit_result(double result);

public:
	~eval_visitor() override = default;
	eval_visitor();
	eval_visitor(const eval_visitor& other) = delete;
	eval_visitor(eval_visitor&& other) noexcept = delete;
	eval_visitor& operator=(const eval_visitor& other) = delete;
	eval_visitor& operator=(eval_visitor&& other) noexcept = delete;
	
	void visit(bin_op& binaryOperator) override;
	void visit(num& number) override;
	void visit(ast_node& node) override;
	void visit(unary_op& unaryOperator) override;
	
	[[nodiscard]] double result() const;
};

template <class T>
double eval_visitor::accept(T& node)
{
	node.accept(*this);

	if (this->m_stack.empty())
	{
		throw interpret_except("Previous node accept did not yield a value");
	}

	const double val = this->m_stack.top();
	this->m_stack.pop();
	return val;
}

