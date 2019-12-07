#pragma once
#include <utility>
#include "ast_common.h"
#include "symbol_types.h"

namespace ast
{
	using procedure_arg = ast_ptr;
	using procedure_arg_list = std::vector<procedure_arg>;

	class routine_call;
}

class procedure_symbol;
class symbol_table_builder;

/**
 * Represents a call to a procedure or a function (which has a return value)
 */
class ast::routine_call : public ast_node {
private:
	procedure_identifier m_procedure_identifier;
	procedure_arg_list m_args;
	
	symbol_type_ptr<procedure_symbol> m_procedure_symbol;

public:
	routine_call(procedure_identifier procedure_identifier, procedure_arg_list shared_ptrs, token token);

	routine_call(const routine_call& other);

	routine_call(routine_call&& other) noexcept;

	routine_call& operator=(const routine_call& other);

	routine_call& operator=(routine_call&& other) noexcept;

	~routine_call() = default;

	[[nodiscard]] const procedure_identifier& procedure_identifier() const;
	[[nodiscard]] const procedure_arg_list& args() const;
	[[nodiscard]] symbol_type_ptr<procedure_symbol> procedure_symbol() const;
	
	void accept(ast_node_visitor& visitor) override;

	friend symbol_table_builder;
};

