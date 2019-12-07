#pragma once
#include "ast_common.h"
#include "type.h"
#include "var_decl.h"

namespace ast
{
	using procedure_param = var_decl;
	using procedure_param_ptr = var_decl_ptr;
	using procedure_param_list = std::vector<procedure_param_ptr>;

	class block;
	class procedure;

	using block_ptr = ast_node_ptr<block>;
}

class symbol_table_builder;
class symbol_table;

/**
 * Represents a procedure or function (which as a return type)
 */
class ast::procedure :
	public ast_node
{
private:
	procedure_identifier m_identifier;
	procedure_param_list m_params;
	type_ptr m_return_type;
	block_ptr m_block;
	
	std::shared_ptr<symbol_table> m_symbol_table;

public:
	procedure(procedure_identifier id, procedure_param_list params, block_ptr block, token token);
	procedure(procedure_identifier id, procedure_param_list params, type_ptr return_type, block_ptr block, token token);

	procedure(const procedure& other) = default;

	procedure(procedure&& other) noexcept;

	procedure& operator=(const procedure& other);

	procedure& operator=(procedure&& other) noexcept;

	~procedure() = default;

	[[nodiscard]] const procedure_identifier& identifier() const;
	[[nodiscard]] const type_ptr& return_type() const;
	[[nodiscard]] bool is_function() const;
	[[nodiscard]] const block_ptr& block() const;
	[[nodiscard]] const procedure_param_list& params() const;
	
	void accept(ast_node_visitor& visitor) override;

	[[nodiscard]] symbol_table& symbol_table() const;

	friend class ::symbol_table_builder;
};

