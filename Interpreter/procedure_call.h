#pragma once
#include <utility>
#include "ast_common.h"

namespace ast
{
	using procedure_arg = ast_ptr;
	using procedure_arg_list = std::vector<procedure_arg>;

	class procedure_call;
}

/**
 * Represents a call to a procedure
 */
class ast::procedure_call : public ast_node {
private:
	procedure_identifier m_procedure_identifier;
	procedure_arg_list m_args;

public:
	procedure_call(procedure_identifier procedure_identifier, procedure_arg_list shared_ptrs);

	procedure_call(const procedure_call& other);

	procedure_call(procedure_call&& other) noexcept;

	procedure_call& operator=(const procedure_call& other);

	procedure_call& operator=(procedure_call&& other) noexcept;

	~procedure_call() = default;

	[[nodiscard]] const procedure_identifier& procedure_identifier() const;
	[[nodiscard]] const procedure_arg_list& args() const;
	
	void accept(ast_node_visitor& visitor) override;
};

