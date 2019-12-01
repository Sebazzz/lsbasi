#include "pch.h"
#include "eval_visitor.h"
#include "interpret_math.h"
#include "builtin_type_symbol.h"
#include "type_impl.h"

void eval_visitor::register_visit_result(const eval_value& result)
{
	this->m_stack.push(result);
}

eval_visitor::eval_visitor() : m_result { builtin_type_symbol::get_for_builtin_type(ast::builtin_type::integer), 0 }
{
}

void eval_visitor::visit(ast::bin_op& binaryOperator)
{
	eval_value result = this->accept(*binaryOperator.left());
	eval_value right_val = this->accept(*binaryOperator.right());

	// Implicit conversion to real
	const bool operator_has_real_conversion = binaryOperator.op() == token_type::divide_real;

	// ... Call to convert result to real if necessary
	if (operator_has_real_conversion)
	{
		const auto real_type_symbol = builtin_type_symbol::get_for_builtin_type(ast::builtin_type::real);
		const auto& real_type_impl = real_type_symbol->type_impl();
		
		real_type_impl.convert_value(right_val, binaryOperator.get_line_info());
		real_type_impl.convert_value(result, binaryOperator.get_line_info());
	}

	// ... Call to convert value to result type if necessary
	right_val.type->type_impl().change_type(result, binaryOperator.get_line_info());
	result.type->type_impl().convert_value(right_val, binaryOperator.get_line_info());

	switch (binaryOperator.op())
	{
	case token_type::plus:
		add_interpret(result, right_val);
		break;
		
	case token_type::minus:
		subtract_interpret(result, right_val);
		break;
		
	case token_type::multiply:
		multiply_interpret(result, right_val);
		break;

	// FIXME: we should probably error out if wrong operator is used
	case token_type::divide_integer:
		divide_interpret(result, right_val);
		break;

	case token_type::divide_real:
		divide_interpret(result, right_val);
		break;
		
	default:
		throw exec_error(L"Invalid operator for bin_op: " + binaryOperator.get_token().to_string(), binaryOperator.get_line_info());
	}

	this->register_visit_result(result);
}

void eval_visitor::visit(ast::num& number)
{
	const auto builtin_symbol = builtin_type_symbol::get_for_builtin_type(number.type());
			this->register_visit_result({builtin_symbol, number.value()});
}

void eval_visitor::visit(ast::ast_node& node)
{
	ast_node_visitor::visit(node);

	this->m_result = this->m_stack.top();
	this->m_stack.pop();
}

void eval_visitor::visit(ast::unary_op& unaryOperator)
{
	const auto expr = unaryOperator.expr();
	auto result = this->accept(*expr);

	switch (unaryOperator.op())
	{
	case token_type::plus:
		// No-op
		break;
	case token_type::minus:
		negate_interpret(result);
		break;
	default:
		throw exec_error(L"Unsupported unary operation: " + unaryOperator.get_token().to_string(), unaryOperator.get_line_info());
	}

	// ReSharper disable CppSomeObjectMembersMightNotBeInitialized - false positive
	this->register_visit_result(result);
	// ReSharper restore CppSomeObjectMembersMightNotBeInitialized
}

eval_value eval_visitor::result() const
{
	return this->m_result;
}
