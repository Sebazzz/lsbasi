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
	eval_value left_val = this->accept(*binaryOperator.left());
	eval_value right_val = this->accept(*binaryOperator.right());

	// Special case for real division
	if (binaryOperator.op() == token_type::divide_real)
	{
		// This lifts all types to a real
		// TODO: I'd like to look this up through the symbol table, but this visitor has no concept of the symbol table
		const auto real_type = builtin_type_symbol::get_for_builtin_type(ast::builtin_type::real);

		if (left_val.type != real_type && real_type->type_impl().supports_implicit_type_conversion_from(left_val.type, token_type::divide_real))
		{
			real_type->type_impl().implicit_type_conversion(left_val);
		}
	}

	// Get the types and do type widening
	const auto& left_val_type_impl = left_val.type->type_impl();
	const auto& right_val_type_impl = right_val.type->type_impl();

	const bool right_to_result_conversion_allowed = left_val_type_impl.supports_implicit_type_conversion_from(right_val.type, binaryOperator.op());
	const bool result_to_right_conversion_allowed = right_val_type_impl.supports_implicit_type_conversion_from(left_val.type, binaryOperator.op());

	// In a type conversion you always have a widening of types if the types are not the same.
	// Otherwise the types are simply not compatible.
	if (!right_to_result_conversion_allowed && !result_to_right_conversion_allowed)
	{
		throw runtime_type_error(L"Unsupported type conversion to " + right_val.type->to_string() + L" from " + left_val.type->to_string(), binaryOperator.get_line_info());
	}

	const type_impl* result_type_impl;
	if (right_to_result_conversion_allowed)
	{
		left_val_type_impl.implicit_type_conversion(right_val);
		result_type_impl = &left_val_type_impl;
	} else if (result_to_right_conversion_allowed)
	{
		right_val_type_impl.implicit_type_conversion(left_val);
		result_type_impl = &right_val_type_impl;
	} else
	{
		throw internal_interpret_except("At this point, both types should be the same");
	}

	try
	{
		result_type_impl->execute_binary_operation(left_val, right_val, binaryOperator.op(), binaryOperator.get_line_info());
	}
	catch (runtime_type_error& e)
	{
		const std::string error_message = "Type error in binary operation: " + wstring_to_string(binaryOperator.get_token().to_string()) + " - " + std::string(e.what());
		throw runtime_type_error(error_message, binaryOperator.get_line_info());
	}
	catch (exec_error& e)
	{
		const std::string error_message = "Unable to execute binary operation: " + wstring_to_string(binaryOperator.get_token().to_string()) + std::string(" - ") + std::string(e.what());
		throw exec_error(error_message, binaryOperator.get_line_info());
	}

	this->register_visit_result(left_val);
}

void eval_visitor::visit(ast::num& number)
{
	const auto builtin_symbol = builtin_type_symbol::get_for_builtin_type(number.type());
			this->register_visit_result({builtin_symbol, number.value()});
}

void eval_visitor::visit(ast::ast_node& node)
{
	ast_node_visitor::visit(node);

	if (this->m_stack.empty() == false)
	{
		this->m_result = this->m_stack.top();
		this->m_stack.pop();
	} else
	{
		throw exec_error(L"Expected the expression to return a value", node.get_line_info());
	}
}

void eval_visitor::visit(ast::unary_op& unaryOperator)
{
	const auto expr = unaryOperator.expr();
	auto result = this->accept(*expr);

	if (unaryOperator.op() == token_type::minus)
	{
		eval_value negative_op = {
			// TODO: I'd like to look this up through the symbol table, but this visitor has no concept of the symbol table
			builtin_type_symbol::get_for_builtin_type(ast::builtin_type::integer),
			expression_value(static_cast<builtin_integer>(-1))
		};
		
		result.type->type_impl().implicit_type_conversion(negative_op);
		result.type->type_impl().execute_binary_operation(result, negative_op, token_type::multiply, unaryOperator.get_line_info());
	} else if (unaryOperator.op() != token_type::plus)
	{
		throw exec_error(L"Unsupported unary operation: " + unaryOperator.get_token().to_string(), unaryOperator.get_line_info());
	}
	
	this->register_visit_result(result);
}

eval_value eval_visitor::result() const
{
	return this->m_result;
}
