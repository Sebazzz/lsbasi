#include "pch.h"
#include "interpreter.h"
#include "stringify_visitor.h"
#include "eval_visitor.h"
#include "exec_visitor.h"
#include "memory_table.h"
#include "builtin_type_symbol.h"

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

	if (dynamic_cast<builtin_type_symbol&>(*result.type.get()).type() == ast::builtin_type::integer)
	{
		return std::to_wstring(result.value.int_val);
	}

	return std::to_wstring(result.value.real_val);
}

std::wstring interpreter::interpret_program()
{
	exec_visitor eval;
	eval.visit(*this->parsed_ast);

	// Load global scope
	this->m_interpretation_info = {
		eval.global_scope(),
		this->parsed_ast
	};
	
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

interpreter::interpretation_info interpreter::get_interpretation_info() const
{
	return this->m_interpretation_info;
}
