#include "pch.h"
#include "rpn_visitor.h"

std::wstring& rpn_visitor::get_string()
{
	return this->string_buf;
}

wchar_t rpn_visitor::op_to_string(token_type token)
{
	switch (token)
	{
	case token_type::plus: return L'+';
	case token_type::minus: return L'-';
	case token_type::multiply: return L'*';
	case token_type::divide_integer: return L'/';
	case token_type::divide_real: return L'/';
	default:
		throw std::logic_error("Unsupported operator");
	}
}

void rpn_visitor::visit(ast::bin_op& binaryOperator)
{
	binaryOperator.left()->accept(*this);
	this->string_buf += L" ";
	
	binaryOperator.right()->accept(*this);
	this->string_buf += L" ";

	this->string_buf += op_to_string(binaryOperator.op());
}

void rpn_visitor::visit(ast::num& number)
{
	this->string_buf.append(number.val_to_string());
}

void rpn_visitor::visit(ast::ast_node& node)
{
	ast_node_visitor::visit(node);
}

void rpn_visitor::visit(ast::unary_op& unaryOperator)
{
	const auto expr = unaryOperator.expr();

	if (unaryOperator.op() == token_type::plus)
	{
		// No effect
		expr->accept(*this);
		return;
	}

	if (unaryOperator.op() == token_type::minus)
	{
		// In the case of a number, we can just consider the number negative
		{
			const auto n = dynamic_cast<ast::num*>(expr.get());

			if (n != nullptr)
			{
				this->string_buf += op_to_string(unaryOperator.op());
				this->visit(*expr);
				return;
			}
		}
		
		// This is actually a multiplication, so...
		expr->accept(*this);
		
		this->string_buf += L" ";
		this->string_buf += L"-1";
		this->string_buf += L" ";
		this->string_buf += op_to_string(unaryOperator.op());
	}

	throw interpret_except("Unsupported unary operator: " + wstring_to_string( unaryOperator.get_token().to_string()), unaryOperator.get_line_info());
}
