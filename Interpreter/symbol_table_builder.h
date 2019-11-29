#pragma once
#include "pch.h"
#include "ast_node_visitor.h"
#include "symbol_table.h"

/**
 * This class visits the AST and assigns symbol tables to the appropriate AST nodes
 */
class symbol_table_builder :
	public ast_node_visitor
{
private:
	std::shared_ptr<symbol_table> m_symbol_table;

	/**
	 * Private constructor to visit child symbol tables
	 */
	symbol_table_builder(const std::wstring& scope_name, symbol_table* parent_table);

	void ensure_symbol_table() const;
	
public:
	~symbol_table_builder() override = default;
	symbol_table_builder() = default;
	symbol_table_builder(const symbol_table_builder& other) = delete;
	symbol_table_builder(symbol_table_builder&& other) noexcept = delete;
	symbol_table_builder& operator=(const symbol_table_builder& other) = delete;
	symbol_table_builder& operator=(symbol_table_builder&& other) noexcept = delete;

	void visit(ast::ast_node& node) override;
	void visit(ast::var& variable) override;
	void visit(ast::var_decl& var_decl) override;
	void visit(ast::program& program) override;
	void visit(ast::procedure& procedure) override;

	[[nodiscard]] std::shared_ptr<symbol_table> symbol_table() const;
};

