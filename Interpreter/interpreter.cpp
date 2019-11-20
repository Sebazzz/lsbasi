#include "interpreter.h"
#include <stdexcept>
#include "stringify_visitor.h"
#include "eval_visitor.h"

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

std::wstring interpreter::stringify_ast()
{
	stringify_visitor visitor;

	this->ensure_parsed();

	ast_node& node = *this->parsed_ast;
	visitor.visit(node);

	return visitor.get_string();
}

std::wstring interpreter::interpret()
{
	this->ensure_parsed();

	eval_visitor eval;
	eval.visit(*this->parsed_ast);
	
	const double result = eval.result();

	if (round(result) == result)
	{
		return std::to_wstring(static_cast<int>(result));
	}

	return std::to_wstring(result);
}