#include "lisp_notation_visitor.h"
#include <stdexcept>

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
	case token_type::divide: return L'/';
	default:
		throw std::logic_error("Unsupported operator");
	}
}

void lisp_notation_visitor::visit(bin_op& binaryOperator)
{
	this->string_buf += L'(';
	this->string_buf += op_to_string(binaryOperator.op());
	this->string_buf += L" ";
	
	binaryOperator.left()->accept(*this);
	this->string_buf += L" ";
	
	binaryOperator.right()->accept(*this);
	this->string_buf += L')';
}

void lisp_notation_visitor::visit(num& number)
{
	this->string_buf.append(std::to_wstring(number.value()));
}

void lisp_notation_visitor::visit(ast_node& node)
{
	ast_node_visitor::visit(node);
}

void lisp_notation_visitor::visit(unary_op& unaryOperator)
{
	unaryOperator.expr()->accept(*this);
}
