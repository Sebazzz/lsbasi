#pragma once
#include "pch.h"
#include "ast_node_visitor.h"
#include "symbol_table.h"

class procedure_symbol;

/**
 * This class visits the AST and assigns symbol tables to the appropriate AST nodes
 */
class symbol_table_builder :
	public ast_node_visitor
{
private:
	std::shared_ptr<symbol_table> m_symbol_table;

	struct procedure_visit_context
	{
		ast::routine& procedure_node;
		symbol_type_ptr<procedure_symbol> procedure_symbol;
	};
	
	std::stack<procedure_visit_context> m_procedures_to_visit;

	/**
	 * Private constructor to visit child symbol tables
	 */
	symbol_table_builder(const std::wstring& scope_name, symbol_table* parent_table);

	void ensure_symbol_table() const;

	/**
	 * Visit procedures that are declared in the current block
	 */
	void visit_leftover_procedures();

	/**
	 * Register a procedure to visit later when all procedures have been declared.
	 */
	void delay_procedure_visit(const procedure_visit_context& procedure_info);

	/**
	 * Initializes the types in a built-in procedure
	 */
	void initialize_builtin_procedure(builtin_procedure_symbol* builtin_procedure);

public:
	~symbol_table_builder() override = default;
	symbol_table_builder() = default;
	symbol_table_builder(const symbol_table_builder& other) = delete;
	symbol_table_builder(symbol_table_builder&& other) noexcept = delete;
	symbol_table_builder& operator=(const symbol_table_builder& other) = delete;
	symbol_table_builder& operator=(symbol_table_builder&& other) noexcept = delete;

	void visit(ast::ast_node& node) override;
	void visit(ast::assignment_target& variable) override;
	void visit(ast::var_decl& var_decl) override;
	void visit(ast::block& block) override;
	void visit(ast::program& program) override;
	void visit(ast::routine& procedure) override;
	void visit(ast::type& type_ref) override;
	void visit(ast::procedure_call& procedure_call) override;
	
	[[nodiscard]] std::shared_ptr<symbol_table> symbol_table() const;
};

