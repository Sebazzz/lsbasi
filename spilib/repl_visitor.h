#pragma once
#include "eval_visitor.h"
#include "interpreter_context.h"

class repl_visitor : public eval_visitor
{
private:
	symbol_table m_repl_symbol_table;
	interpreter_context& m_interpreter_context;
	std::unique_ptr<scope_context> m_repl_scope;
	
protected:
	scope_context& get_current_scope() override;
	interpreter_context& get_interpreter_context() const override;
	
public:
	~repl_visitor() override = default;
	repl_visitor(interpreter_context& interpreter_context);
	repl_visitor(const repl_visitor& other) = delete;
	repl_visitor(repl_visitor&& other) noexcept = delete;
	repl_visitor& operator=(const repl_visitor& other) = delete;
	repl_visitor& operator=(repl_visitor&& other) noexcept = delete;

	void visit(ast::ast_node& node) override;
};

