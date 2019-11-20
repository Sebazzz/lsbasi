#include "rpn_visitor.h"
#include <stdexcept>

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
	case token_type::divide: return L'/';
	default:
		throw std::logic_error("Unsupported operator");
	}
}

void rpn_visitor::visit(bin_op& binaryOperator)
{
	binaryOperator.left()->accept(*this);
	this->string_buf += L" ";
	
	binaryOperator.right()->accept(*this);
	this->string_buf += L" ";

	this->string_buf += op_to_string(binaryOperator.op());
}

void rpn_visitor::visit(num& number)
{
	this->string_buf.append(std::to_wstring(number.value()));
}

void rpn_visitor::visit(ast_node& node)
{
	ast_node_visitor::visit(node);
}
