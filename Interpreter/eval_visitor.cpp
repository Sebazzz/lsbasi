#include "eval_visitor.h"
#include "interpret_math.h"

void eval_visitor::register_visit_result(double result)
{
	this->m_stack.push(result);
}

eval_visitor::eval_visitor() : m_result(0)
{
}

void eval_visitor::visit(bin_op& binaryOperator)
{
	double result = this->accept(*binaryOperator.left());
	const double rightVal = this->accept(*binaryOperator.right());

	switch (binaryOperator.op())
	{
	case token_type::plus:
		add_interpret(result, rightVal);
		break;
		
	case token_type::minus:
		subtract_interpret(result, rightVal);
		break;
		
	case token_type::multiply:
		// TODO: implement overflow detection
		result *= rightVal;
		break;
		
	case token_type::divide:
		divide_interpret(result, static_cast<double>(rightVal));
		break;
		
	default:
		throw interpret_except("Invalid operator for bin_op");
	}

	this->register_visit_result(result);
}

void eval_visitor::visit(num& number)
{
	this->register_visit_result(number.value());
}

void eval_visitor::visit(ast_node& node)
{
	ast_node_visitor::visit(node);

	this->m_result = this->m_stack.top();
	this->m_stack.pop();
}

void eval_visitor::visit(unary_op& unaryOperator)
{
	const auto expr = unaryOperator.expr();
	auto result = this->accept(*expr);

	switch (unaryOperator.op())
	{
	case token_type::plus:
		// No-op
		break;
	case token_type::minus:
		result *= -1;
		break;
	default:
		throw interpret_except("Unsupported unary operation");
	}

	this->register_visit_result(result);
}

double eval_visitor::result() const
{
	return this->m_result;
}
