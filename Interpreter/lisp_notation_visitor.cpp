#include "pch.h"
#include "lisp_notation_visitor.h"

std::wstring& lisp_notation_visitor::get_string()
{
	return this->string_buf;
}

wchar_t lisp_notation_visitor::op_to_string(token_type token)
{
	switch (token)
	{
	case token_type::plus: return L'+';
	case token_type::minus: return L'-';
	case token_type::multiply: return L'*';
	case token_type::divide_integer: return L'/';
	default:
		throw std::logic_error("Unsupported operator");
	}
}

void lisp_notation_visitor::visit(ast::bin_op& binaryOperator)
{
	this->string_buf += L'(';
	this->string_buf += op_to_string(binaryOperator.op());
	this->string_buf += L" ";
	
	binaryOperator.left()->accept(*this);
	this->string_buf += L" ";
	
	binaryOperator.right()->accept(*this);
	this->string_buf += L')';
}

void lisp_notation_visitor::visit(ast::num& number)
{
	this->string_buf.append(number.val_to_string());
}

void lisp_notation_visitor::visit(ast::ast_node& node)
{
	ast_node_visitor::visit(node);
}

void lisp_notation_visitor::visit(ast::unary_op& unaryOperator)
{
	unaryOperator.expr()->accept(*this);
}
