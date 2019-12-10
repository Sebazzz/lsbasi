#pragma once
#include "ast_common.h"
#include "type.h"
#include "var_decl.h"

namespace ast
{
	using routine_param = var_decl;
	using routine_param_ptr = var_decl_ptr;
	using routine_param_list = std::vector<routine_param_ptr>;

	class block;
	class routine;

	using block_ptr = ast_node_ptr<block>;
}

class symbol_table_builder;
class symbol_table;

/**
 * Represents a procedure or function (which has a return type)
 */
class ast::routine :
	public ast_node
{
private:
	procedure_identifier m_identifier;
	routine_param_list m_params;
	type_ptr m_return_type;
	block_ptr m_block;
	
	std::shared_ptr<symbol_table> m_symbol_table;

public:
	routine(procedure_identifier id, routine_param_list params, block_ptr block, token token);
	routine(procedure_identifier id, routine_param_list params, type_ptr return_type, block_ptr block, token token);

	routine(const routine& other) = default;

	routine(routine&& other) noexcept;

	routine& operator=(const routine& other);

	routine& operator=(routine&& other) noexcept;

	~routine() = default;

	[[nodiscard]] const procedure_identifier& identifier() const;
	[[nodiscard]] const type_ptr& return_type() const;
	[[nodiscard]] bool is_function() const;
	[[nodiscard]] const block_ptr& block() const;
	[[nodiscard]] const routine_param_list& params() const;
	
	void accept(ast_node_visitor& visitor) override;

	[[nodiscard]] symbol_table& symbol_table() const;

	friend class ::symbol_table_builder;
};

