#include "pch.h"
#include "eval_visitor.h"
#include "interpret_math.h"

void eval_visitor::register_visit_result(eval_value result)
{
	this->m_stack.push(result);
}

eval_visitor::eval_visitor() : m_result { ast::builtin_type::integer, 0 }
{
}

void eval_visitor::visit(ast::bin_op& binaryOperator)
{
	eval_value result = this->accept(*binaryOperator.left());
	eval_value right_val = this->accept(*binaryOperator.right());

	// Implicit conversion to real
	const bool operator_has_real_conversion = binaryOperator.op() == token_type::divide_real;
	if (result.type == ast::builtin_type::real && right_val.type != result.type || operator_has_real_conversion)
	{
		if (right_val.type == ast::builtin_type::integer)
		{
			right_val.value.real_val = right_val.value.int_val;
			right_val.type = ast::builtin_type::real;
		}
	}

	if (result.type != right_val.type && result.type == ast::builtin_type::integer || operator_has_real_conversion)
	{
		if (result.type == ast::builtin_type::integer){
			result.value.real_val = result.value.int_val;
			result.type = ast::builtin_type::real;
		}
	}

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
		throw interpret_except("Invalid operator for bin_op", std::to_string(static_cast<int>(binaryOperator.op())));
	}

	this->register_visit_result(result);
}

void eval_visitor::visit(ast::num& number)
{
	switch (number.type())
	{
	case ast::builtin_type::integer:
	case ast::builtin_type::real:
		this->register_visit_result({number.type(), number.value()});
		break;
	default:
		throw interpret_except("Unsupported number type", std::to_string(static_cast<int>(number.type())))
		;
	}
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
		switch (result.type)
		{
		case ast::builtin_type::integer:
			result.value.int_val *= -1;
			break;
		case ast::builtin_type::real:
			result.value.real_val *= -1;
			break;
		default: ;
		}
		break;
	default:
		throw interpret_except("Unsupported unary operation", std::to_string(static_cast<int>(unaryOperator.op())));
	}

	// ReSharper disable CppSomeObjectMembersMightNotBeInitialized - false positive
	this->register_visit_result(result);
	// ReSharper restore CppSomeObjectMembersMightNotBeInitialized
}

eval_visitor::eval_value eval_visitor::result() const
{
	return this->m_result;
}
