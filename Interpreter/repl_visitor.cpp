#include "pch.h"
#include "memory_table.h"
#include "repl_visitor.h"

scope_context& repl_visitor::get_current_scope()
{
	return *this->m_repl_scope;
}

interpreter_context& repl_visitor::get_interpreter_context() const
{
	return this->m_interpreter_context;
}

void repl_visitor::visit(ast::ast_node& node)
{
	eval_visitor::visit(node);

	if (!this->has_result())
	{
		throw exec_error(L"Expected the expression to return a value", node.get_line_info());
	}
}

repl_visitor::repl_visitor(interpreter_context& interpreter_context) : m_repl_symbol_table(L"REPL"), m_interpreter_context(interpreter_context)
{
	this->m_repl_scope = std::make_unique<scope_context>(this->m_repl_symbol_table, memory_table::create_from_parent_scope(nullptr));
}
