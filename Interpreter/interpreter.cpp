#include "pch.h"
#include "interpreter.h"
#include "stringify_visitor.h"
#include "eval_visitor.h"
#include "exec_visitor.h"

void interpreter::ensure_parsed()
{
	if (!this->parsed_ast)
	{
		this->do_parse();
	}
}

void interpreter::do_parse()
{
	if (this->m_repl_mode)
	{
		this->parsed_ast = this->parser.parse_repl();
	} else
	{
		this->parsed_ast = this->parser.parse();
	}
}

std::wstring interpreter::tokenize()
{
	return this->parser.stringify_parse_tree();
}

std::wstring interpreter::stringify_ast()
{
	stringify_visitor visitor;

	this->ensure_parsed();

	ast::ast_node& node = *this->parsed_ast;
	visitor.visit(node);

	return visitor.get_string();
}

std::wstring interpreter::interpret_repl() const
{
	eval_visitor eval;
	eval.visit(*this->parsed_ast);
	
	const auto result = eval.result();

	if (result.type == ast::var_type::integer)
	{
		return std::to_wstring(result.value.int_val);
	}

	return std::to_wstring(result.value.real_val);
}

std::wstring interpreter::interpret_program() const
{
	exec_visitor eval;
	eval.visit(*this->parsed_ast);

	// TODO: output?
	
	return std::wstring(L"done");
}

std::wstring interpreter::interpret()
{
	this->ensure_parsed();

	if (this->m_repl_mode)
	{
		return interpret_repl();
	}
	
	return interpret_program();
}
