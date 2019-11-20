#include "interpreter.h"
#include <stdexcept>
#include "interpret_except.h"
#include "interpret_math.h"

void interpreter::ensure_parsed()
{
	if (!this->parsed_ast)
	{
		this->do_parse();
	}
}

void interpreter::do_parse()
{
	this->parsed_ast = this->parser.parse();
}

std::wstring interpreter::tokenize()
{
	return this->parser.stringify_parse_tree();
}

std::wstring interpreter::interpret()
{
	this->ensure_parsed();

	const double result = handle_ast_node(*this->parsed_ast);

	if (round(result) == result)
	{
		return std::to_wstring(static_cast<int>(result));
	}

	return std::to_wstring(result);
}

int interpreter::handle_num(num& node)
{
	return node.value();
}

double interpreter::handle_bin_op(bin_op& node) const
{
	double result = handle_ast_node(*node.left().get());
	const double rightVal = handle_ast_node(*node.right().get());
	
	switch (node.op())
	{
	case token_type::plus:
		add_interpret(result, rightVal);
		break;
		
	case token_type::minus:
		subtract_interpret(result, rightVal);
		break;
		
	case token_type::multiply:
		// TODO: implement overflow detection
		result *= rightVal;
		break;
		
	case token_type::divide:
		divide_interpret(result, static_cast<double>(rightVal));
		break;
		
	default:
		throw interpret_except("Invalid operator for bin_op");
	}

	return result;
}

double interpreter::handle_ast_node(ast_node& node) const
{
	if (typeid(node) == typeid(bin_op)){
		auto& binaryOp = dynamic_cast<bin_op&>(node);

		return handle_bin_op(binaryOp);
	}

	if (typeid(node) == typeid(num)){
		auto& numNode = dynamic_cast<num&>(node);

		return handle_num(numNode);
	}

	throw interpret_except("Unknown or unsupported syntax node");
}