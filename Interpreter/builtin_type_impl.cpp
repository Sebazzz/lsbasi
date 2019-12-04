#include "pch.h"
#include "builtin_type_impl.h"
#include "interpret_math.h"
#include "var.h"

void builtin_type_impl::assign_self_type(eval_value& eval_value, type_operation_context& type_operation_context) const
{
	eval_value.type = type_operation_context.symbol_table.get(this->m_symbol->type());
}

void builtin_type_impl::implicit_type_conversion(ast::expression_value&, builtin_type_symbol*, type_operation_context&) const
{
	throw internal_interpret_except("Type conversion is not implemented");
}

builtin_type_impl::builtin_type_impl(builtin_type_symbol* builtin_type_symbol): m_symbol(builtin_type_symbol)
{
}

void builtin_type_impl::execute_binary_operation(eval_value& result, const eval_value& right_val, token_type op, type_operation_context& type_operation_context) const
{
	const auto& current_type = result.type;

	// Try to find built-in type
	{
		const auto builtin_type = dynamic_cast<builtin_type_symbol*>(current_type.get());

		if (builtin_type != nullptr && 
			(builtin_type->type() == this->m_symbol->type() || this->supports_type_conversion_from(builtin_type->type(), token_type::assign)))
		{
			this->execute_binary_operation(result, right_val.value, *builtin_type, op, type_operation_context);
			return;
		}
	}

	// It was not a built-in type.
	// This type conversion is not supported (note: at time of writing we don't have anything except built-in types)
	throw runtime_type_error(L"Unsupported type conversion to " + 
		this->m_symbol->to_string() + L" from " + current_type->to_string(), type_operation_context.line_info);
}

bool builtin_type_impl::supports_implicit_type_conversion_from(symbol_type_ptr<type_symbol> type, token_type op) const
{
	const auto builtin_type = dynamic_cast<builtin_type_symbol*>(type.get());

	if (builtin_type != nullptr)
	{
		// Assignment from same type is always allowed
		return builtin_type->type() == this->m_symbol->type() || 
			   this->supports_type_conversion_from(builtin_type->type(), op); // Else the derived impl may know more
	}

	return false;
}

void builtin_type_impl::implicit_type_conversion(eval_value& value, type_operation_context& type_operation_context) const
{
	if (!this->supports_implicit_type_conversion_from(value.type, token_type::assign))
	{
		throw internal_interpret_except("Support method should have been called");
	}

	const auto builtin_type = dynamic_cast<builtin_type_symbol*>(value.type.get());
	if (builtin_type == nullptr)
	{
		throw internal_interpret_except("Cannot find built-in type in evaluation value");
	}
	
	if (builtin_type->type() == this->m_symbol->type())
	{
		return;
	}

	this->implicit_type_conversion(value.value, builtin_type, type_operation_context);
	this->assign_self_type(value, type_operation_context);
}

bool builtin_real_impl::supports_type_conversion_from(ast::builtin_type from_type, token_type) const
{
	// Widening from integer from real is allowed
	return from_type == ast::builtin_type::integer;
}

void builtin_real_impl::execute_binary_operation(eval_value& result, const expression_value& from, const builtin_type_symbol& right_val, token_type op, type_operation_context&) const
{
	// Implicit conversion to real
	expression_value from_op = from;
	if (right_val.type() == ast::builtin_type::integer)
	{
		from_op.real_val = from.int_val;
	}

	switch (op) {
	case token_type::plus:
		add_interpret(result.value.real_val, from_op.real_val);
		break;
		
	case token_type::minus:
		subtract_interpret(result.value.real_val, from_op.real_val);
		break;
		
	case token_type::multiply:
		multiply_interpret(result.value.real_val, from_op.real_val);
		break;

	// FIXME: we should probably error out if wrong operator is used
	case token_type::divide_integer:
		divide_interpret(result.value.real_val, from_op.real_val);
		break;

	case token_type::divide_real:
		divide_interpret(result.value.real_val, from_op.real_val);
		break;
		
	default:
		throw exec_error("Invalid operator for bin_op: " + std::to_string(static_cast<int>(op)), {-1,-1});
	}
}

void builtin_real_impl::implicit_type_conversion(ast::expression_value& value, builtin_type_symbol* builtin_type, type_operation_context&) const
{
	if (builtin_type->type() == ast::builtin_type::integer)
	{
		value.real_val = value.int_val;
	} else
	{
		throw internal_interpret_except("Type conversion is not allowed");
	}
}

builtin_real_impl::builtin_real_impl(builtin_type_symbol* builtin_type_symbol):builtin_type_impl(builtin_type_symbol)
{
	
}

bool builtin_integer_impl::supports_type_conversion_from(ast::builtin_type from_type, token_type) const
{
	return from_type == ast::builtin_type::integer;
}

void builtin_integer_impl::execute_binary_operation(eval_value& result, const expression_value& from, const builtin_type_symbol&, token_type op, type_operation_context&) const
{
	switch (op) {
	case token_type::plus:
		add_interpret(result.value.int_val, from.int_val);
		break;
		
	case token_type::minus:
		subtract_interpret(result.value.int_val, from.int_val);
		break;
		
	case token_type::multiply:
		multiply_interpret(result.value.int_val, from.int_val);
		break;

	case token_type::divide_integer:
		divide_interpret(result.value.int_val, from.int_val);
		break;

	case token_type::divide_real:
		throw exec_error("REAL division not supported for integers", {-1,-1});
		
	default:
		throw exec_error("Invalid operator for bin_op: " + std::to_string(static_cast<int>(op)), {-1,-1});
	}
}

builtin_integer_impl::builtin_integer_impl(builtin_type_symbol* builtin_type_symbol):builtin_type_impl(builtin_type_symbol)
{
}

bool builtin_string_impl::supports_type_conversion_from(ast::builtin_type from_type, token_type) const
{
	return from_type == ast::builtin_type::string; // This method will actually never be called
}

void builtin_string_impl::execute_binary_operation(eval_value& current, const expression_value& from, const builtin_type_symbol&, token_type op, type_operation_context& type_operation_context) const
{
	if (op != token_type::plus)
	{
		throw exec_error("Invalid operator for string", {-1,-1});
	}

	// Add this to the string pool
	auto new_str = builtin_string(*current.value.string_ptr_val);
	new_str.append(*from.string_ptr_val);
	current.value.string_ptr_val = type_operation_context.interpreter_context.string_pool.get_or_add(new_str);
}

builtin_string_impl::builtin_string_impl(builtin_type_symbol* builtin_type_symbol):builtin_type_impl(builtin_type_symbol)
{
}
