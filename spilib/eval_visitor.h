#pragma once
#include "pch.h"
#include "ast_node_visitor.h"
#include "ast_common.h"
#include "eval_value.h"
#include "scope_context.h"
#include "interpreter_context.h"
#include <optional>

class eval_visitor abstract : public ast_node_visitor
{
private:
	/*
	 * Hold the last evaluated item. This to work around that we don't have visitor with return type.
	 */
	std::optional<eval_value> m_result;

protected:
	/**
	 * Helper function to visit the specified node and get the result back
	 */
	template <class T>
	eval_value visit_with_result(T& node);

	/**
	 * Helper function to visit the specified node which returns no value (or we don't care about the value)
	 */
	template <class T>
	void visit_with_void_result(T& node);

	/**
	 * Helper method to register the result for further up the call stack
	 */
	void register_visit_result(const eval_value& result);

	/**
	 * Helper method to register that from the current function there is no result (void, if you like)
	 */
	void register_void_result();

	/**
	 * Gets the current scope. In REPL mode this may be fixed, in full interpretation mode this may vary.
	 */
	virtual scope_context& get_current_scope() = 0;

	/**
	 * Gets the interpretation context
	 */
	virtual interpreter_context& get_interpreter_context() const = 0;

	/**
	 * Gets whether the last evaluation returned a result
	 */
	bool has_result() const;

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
eval_value eval_visitor::visit_with_result(T& node)
{
	node.accept(*this);

	if (!this->m_result)
	{
		throw internal_interpret_except(std::string("Previous node accept did not yield a value: ") + typeid(node).name(), node.get_line_info());
	}

	const eval_value val = this->m_result.value();
	this->m_result.reset();
	return val;
}

template <class T>
void eval_visitor::visit_with_void_result(T& node)
{
	node.accept(*this);
	this->m_result.reset();
}

