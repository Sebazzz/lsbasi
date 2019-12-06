#include "pch.h"
#include "eval_visitor.h"
#include "interpret_math.h"
#include "builtin_type_symbol.h"
#include "type_impl.h"

void eval_visitor::register_visit_result(const eval_value& result)
{
	this->m_result = result;
}

void eval_visitor::register_void_result()
{
	this->m_result.reset();
}

bool eval_visitor::has_result() const
{
	// "!!this->m_result" would also work, but look odd
	return this->m_result.has_value();
}

eval_visitor::eval_visitor()
{
	this->register_void_result();
}

void eval_visitor::visit(ast::bin_op& binaryOperator)
{
	eval_value left_val = this->visit_with_result(*binaryOperator.left());
	eval_value right_val = this->visit_with_result(*binaryOperator.right());

	// Type context required for type operations
	type_operation_context type_operation_context = {
		this->get_interpreter_context(),
		this->get_current_scope().symbols,
		binaryOperator.get_line_info()
	};
	
	// Special case for real division
	if (binaryOperator.op() == token_type::divide_real)
	{
		// This lifts all types to a real
		const auto real_type = type_operation_context.symbol_table.get(ast::builtin_type::real);

		if (left_val.type != real_type && real_type->type_impl().supports_implicit_type_conversion_from(left_val.type, token_type::divide_real))
		{
			real_type->type_impl().implicit_type_conversion(left_val, type_operation_context);
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
		left_val_type_impl.implicit_type_conversion(right_val, type_operation_context);
		result_type_impl = &left_val_type_impl;
	} else if (result_to_right_conversion_allowed)
	{
		right_val_type_impl.implicit_type_conversion(left_val, type_operation_context);
		result_type_impl = &right_val_type_impl;
	} else
	{
		throw internal_interpret_except("At this point, both types should be the same");
	}

	try
	{
		result_type_impl->execute_binary_operation(left_val, right_val, binaryOperator.op(), type_operation_context);
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
}

void eval_visitor::visit(ast::unary_op& unaryOperator)
{
	const auto expr = unaryOperator.expr();
	auto result = this->visit_with_result(*expr);

	if (unaryOperator.op() == token_type::minus)
	{
		const auto& scope_context = this->get_current_scope();
		
		eval_value negative_op = {
			scope_context.symbols.get(ast::builtin_type::integer),
			expression_value(static_cast<builtin_integer>(-1))
		};

		type_operation_context type_operation_context = {
			this->get_interpreter_context(),
			scope_context.symbols,
			unaryOperator.get_line_info()
		};
		
		result.type->type_impl().implicit_type_conversion(negative_op, type_operation_context);
		result.type->type_impl().execute_binary_operation(result, negative_op, token_type::multiply, type_operation_context);
	} else if (unaryOperator.op() != token_type::plus)
	{
		throw exec_error(L"Unsupported unary operation: " + unaryOperator.get_token().to_string(), unaryOperator.get_line_info());
	}
	
	this->register_visit_result(result);
}

eval_value eval_visitor::result() const
{
	return this->m_result.value();
}
