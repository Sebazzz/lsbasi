#pragma once
#include <utility>
#include "ast_common.h"
#include "symbol_types.h"

namespace ast
{
	using procedure_arg = ast_ptr;
	using procedure_arg_list = std::vector<procedure_arg>;

	class procedure_call;
}

class procedure_symbol;
class symbol_table_builder;

/**
 * Represents a call to a procedure
 */
class ast::procedure_call : public ast_node {
private:
	procedure_identifier m_procedure_identifier;
	procedure_arg_list m_args;
	
	symbol_type_ptr<procedure_symbol> m_procedure_symbol;

public:
	procedure_call(procedure_identifier procedure_identifier, procedure_arg_list shared_ptrs, token token);

	procedure_call(const procedure_call& other);

	procedure_call(procedure_call&& other) noexcept;

	procedure_call& operator=(const procedure_call& other);

	procedure_call& operator=(procedure_call&& other) noexcept;

	~procedure_call() = default;

	[[nodiscard]] const procedure_identifier& procedure_identifier() const;
	[[nodiscard]] const procedure_arg_list& args() const;
	[[nodiscard]] symbol_type_ptr<procedure_symbol> procedure_symbol() const;
	
	void accept(ast_node_visitor& visitor) override;

	friend symbol_table_builder;
};

