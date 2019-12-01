#include "pch.h"
#include "builtin_type_impl.h"


/**
 * We put in a shared pointer but since we don't own the object, don't delete it.
 */
void null_deleter(type_symbol*) {}

void builtin_type_impl::assign_self_type(eval_value& eval_value) const
{
	eval_value.type = std::shared_ptr<type_symbol>(this->m_symbol, null_deleter);
}

void builtin_type_impl::convert_value(ast::expression_value& expr_value, builtin_type_symbol& expr_type, line_info line_info) const
{
	// No conversion if same type
	if (expr_type.type() == this->m_symbol->type())
	{
		switch (expr_type.type())
		{
			case ast::builtin_type::integer:
			case ast::builtin_type::real:
				return;

			default:
				throw runtime_type_error(L"Attempting to assign expression of type " + expr_type.to_string() + L" to " + this->m_symbol->to_string(), line_info);
		}
	}
	
	// Implicit conversion if destination is of type real
	if (expr_type.type() == ast::builtin_type::integer && this->m_symbol->type() == ast::builtin_type::real)
	{
		expr_value.real_val = expr_value.int_val;
		return;
	}

	// Implicit conversion from real to int is not allowed
	if (expr_type.type() == ast::builtin_type::real && this->m_symbol->type() == ast::builtin_type::integer)
	{
		throw runtime_type_error(L"Attempting to convert expression of type " + expr_type.to_string() + L" to " + this->m_symbol->to_string(), line_info);
	}
}

builtin_type_impl::builtin_type_impl(builtin_type_symbol* builtin_type_symbol): m_symbol(builtin_type_symbol)
{
}

void builtin_type_impl::convert_value(eval_value& eval_value, line_info line_info) const
{
	const auto& current_type = eval_value.type;

	// Try to find built-in type
	{
		const auto builtin_type = dynamic_cast<builtin_type_symbol*>(current_type.get());

		if (builtin_type != nullptr)
		{
			this->convert_value(eval_value.value, *builtin_type, line_info);
			this->assign_self_type(eval_value);
			return;
		}
	}

	// It was not a built-in type.
	// This type conversion is not supported (note: at time of writing we don't have anything except built-in types)
	throw runtime_type_error(L"Unsupported type conversion to " + 
		this->m_symbol->to_string() + L" from " + current_type->to_string(), line_info);
}

void builtin_type_impl::change_type(eval_value& eval_value, line_info) const
{
	const auto& current_type = eval_value.type;
	
	// Try to find built-in type
	const auto eval_value_builtin_type = dynamic_cast<builtin_type_symbol*>(current_type.get());

	{
		if (eval_value_builtin_type != nullptr)
		{
			if (eval_value_builtin_type->type() == this->m_symbol->type() ||
				eval_value_builtin_type->type() == ast::builtin_type::real /*Widest type available*/)
			{
				// Nothing to do
				return;
			}

			// At this point we are a int or real
			// The incoming value is a int or unknown

			switch (this->m_symbol->type())
			{
				
			case ast::builtin_type::integer:
				eval_value.value.int_val = static_cast<int>(eval_value.value.real_val);
				break;
				
			case ast::builtin_type::real:
				eval_value.value.real_val = eval_value.value.int_val;
				break;

				// Ignore unhandled types
			default:
				return;
			}
			
			this->assign_self_type(eval_value);
		}
	}
}
