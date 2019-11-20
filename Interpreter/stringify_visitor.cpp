#include "stringify_visitor.h"

std::wstring& stringify_visitor::get_string()
{
	return this->string_buf;
}

void stringify_visitor::visit(bin_op& binaryOperator)
{
	this->string_buf += L"[";

	binaryOperator.left()->accept(*this);

	this->string_buf += L" ";
	this->string_buf += token(binaryOperator.op(), std::wstring()).to_string();
	this->string_buf += L" ";
	
	binaryOperator.right()->accept(*this);

	this->string_buf += L"]";
}

void stringify_visitor::visit(num& number)
{
	this->string_buf.append(std::to_wstring(number.value()));
}

void stringify_visitor::visit(ast_node& node)
{
	ast_node_visitor::visit(node);
}
