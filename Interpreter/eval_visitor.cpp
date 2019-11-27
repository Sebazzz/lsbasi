#include "eval_visitor.h"
#include "interpret_math.h"

void eval_visitor::register_visit_result(double result)
{
	this->m_stack.push(result);
}

eval_visitor::eval_visitor() : m_result(0)
{
}

void eval_visitor::visit(ast::bin_op& binaryOperator)
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
		
	case token_type::divide_integer:
		divide_interpret(result, static_cast<double>(rightVal));
		break;

	// FIXME: until we implement real division
	case token_type::divide_real:
		divide_interpret(result, static_cast<double>(rightVal));
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
	case ast::var_type::integer:
		this->register_visit_result(number.value_integer());
		break;
	case ast::var_type::real:
		this->register_visit_result(number.value_real());
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
		result *= -1;
		break;
	default:
		throw interpret_except("Unsupported unary operation", std::to_string(static_cast<int>(unaryOperator.op())));
	}

	this->register_visit_result(result);
}

double eval_visitor::result() const
{
	return this->m_result;
}
