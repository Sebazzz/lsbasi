#include "stringify_visitor.h"
#include "token.h"
#include "interpret_except.h"

std::wstring& stringify_visitor::get_string()
{
	return this->string_buf;
}

void stringify_visitor::visit(ast::bin_op& binaryOperator)
{
	this->string_buf += L"[";

	binaryOperator.left()->accept(*this);

	this->string_buf += L" ";
	this->string_buf += token(binaryOperator.op(), std::wstring()).to_string();
	this->string_buf += L" ";
	
	binaryOperator.right()->accept(*this);

	this->string_buf += L"]";
}

void stringify_visitor::visit(ast::num& number)
{
	switch (number.type())
	{
	case ast::var_type::integer:
		this->string_buf.append(std::to_wstring(number.value_integer()));
		break;
	case ast::var_type::real:
		this->string_buf.append(std::to_wstring(number.value_real()));
		break;
	default:
		throw interpret_except("Unsupported number type", std::to_string(static_cast<int>(number.type())))
		;
	}
}

void stringify_visitor::visit(ast::ast_node& node)
{
	ast_node_visitor::visit(node);
}

void stringify_visitor::visit(ast::unary_op& unaryOperator)
{
	this->string_buf.append(token(unaryOperator.op(), std::wstring()).to_string());
	unaryOperator.expr()->accept(*this);
}

void stringify_visitor::visit(ast::compound& compound)
{
	for (const auto& statement : compound.statements())
	{
		statement->accept(*this);
		
		this->string_buf.append(L"\r\n");
	}
}

void stringify_visitor::visit(ast::assign& assign)
{
	assign.left()->accept(*this);
	
	this->string_buf.append(L" <- ");

	assign.right()->accept(*this);
}

void stringify_visitor::visit(ast::var& variable)
{
	this->string_buf.append(variable.identifier());
}

void stringify_visitor::visit(ast::empty&)
{
	this->string_buf.append(L" (empty) ");
}
