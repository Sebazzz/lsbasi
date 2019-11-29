#pragma once
#include "pch.h"
#include "ast_node_visitor.h"
#include "symbol_table.h"

/**
 * This class visits the AST and assigns symbol tables to the appropriate AST nodes
 */
class symbol_reference_visitor :
	public ast_node_visitor
{
private:
	std::shared_ptr<symbol_table> m_symbol_table;

	/**
	 * Private constructor to visit child symbol tables
	 */
	symbol_reference_visitor(const std::wstring& scope_name, symbol_table* parent_table);

	void ensure_symbol_table() const;
	
public:
	~symbol_reference_visitor() override = default;
	symbol_reference_visitor() = default;
	symbol_reference_visitor(const symbol_reference_visitor& other) = delete;
	symbol_reference_visitor(symbol_reference_visitor&& other) noexcept = delete;
	symbol_reference_visitor& operator=(const symbol_reference_visitor& other) = delete;
	symbol_reference_visitor& operator=(symbol_reference_visitor&& other) noexcept = delete;

	void visit(ast::ast_node& node) override;
	void visit(ast::var& variable) override;
	void visit(ast::var_decl& var_decl) override;
	void visit(ast::program& program) override;
	void visit(ast::procedure& procedure) override;

	[[nodiscard]] std::shared_ptr<symbol_table> symbol_table() const;
};

