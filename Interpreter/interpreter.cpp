#include "pch.h"
#include "interpreter.h"
#include "stringify_visitor.h"
#include "eval_visitor.h"
#include "exec_visitor.h"
#include "builtin_type_symbol.h"
#include "repl_visitor.h"

void interpreter::ensure_parsed()
{
	if (!this->m_parsed_ast)
	{
		this->do_parse();
	}
}

void interpreter::do_parse()
{
	if (this->m_repl_mode)
	{
		this->m_parsed_ast = this->m_parser.parse_repl();
	} else
	{
		this->m_parsed_ast = this->m_parser.parse();
	}
}

interpreter::interpreter(lexer_input_stream input_stream, bool repl_mode): m_context_ptr(std::make_shared<interpreter_context>()), m_parser(input_stream, m_context_ptr), m_repl_mode(repl_mode)
{
}

std::wstring interpreter::tokenize()
{
	return this->m_parser.stringify_parse_tree();
}

std::wstring interpreter::stringify_ast()
{
	stringify_visitor visitor;

	this->ensure_parsed();

	ast::ast_node& node = *this->m_parsed_ast;
	visitor.visit(node);

	return visitor.get_string();
}

std::wstring interpreter::interpret_repl() const
{
	repl_visitor eval(*this->m_context_ptr);
	eval.visit(*this->m_parsed_ast);
	
	const auto result = eval.result();
	const auto& result_type = dynamic_cast<builtin_type_symbol&>(*result.type.get());

	switch (result_type.type())
	{
	case ast::builtin_type::integer:
		return std::to_wstring(result.value.int_val);
	case ast::builtin_type::real:
		return std::to_wstring(result.value.real_val);
	case ast::builtin_type::string:
		// Copy the value because the memory is owned by the interpreter which may go out of scope
		return *result.value.string_ptr_val;
	default:
		return L"Unsupported expression result";
	}
}

std::wstring interpreter::interpret_program()
{
	exec_visitor eval(*this->m_context_ptr);
	eval.visit(*this->m_parsed_ast);

	// Load global scope
	this->m_interpretation_info = {
		eval.global_scope(),
		this->m_context_ptr,
		this->m_parsed_ast
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
