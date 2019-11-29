#pragma once
#include "pch.h"
#include "ast_node_visitor.h"
#include "memory_table.h"

class eval_visitor : public ast_node_visitor
{
private:
	memory_contents m_result;

	/*
	 * Hold the last evaluated items. This to work around that we don't have visitor with return type.
	 */
	std::stack<memory_contents> m_stack;

protected:
	/**
	 * Helper function to visit the specified node and get the result back
	 */
	template <class T>
	memory_contents accept(T& node);

	/**
	 * Helper method to register the result for further up the call stack
	 */
	void register_visit_result(memory_contents result);

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
	
	[[nodiscard]] memory_contents result() const;
};

template <class T>
memory_contents eval_visitor::accept(T& node)
{
	node.accept(*this);

	if (this->m_stack.empty())
	{
		throw interpret_except("Previous node accept did not yield a value", typeid(node).name());
	}

	const memory_contents val = this->m_stack.top();
	this->m_stack.pop();
	return val;
}

