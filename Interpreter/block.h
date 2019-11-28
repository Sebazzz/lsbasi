#pragma once
#include "ast_common.h"

namespace ast
{
	using procedure_decl_list = std::vector<procedure_decl_ptr>;
	using var_decl_list = std::vector<var_decl_ptr>;
	
	class block;
}

class ast::block final : public ast_node
{
private:
	compound_ptr m_compound;
	var_decl_list m_var_declarations;
	procedure_decl_list m_procedure_declarations;

public:
	block(var_decl_list var_declarations, procedure_decl_list procedure_declarations, compound_ptr compound);

	block(const block& other) = default;

	block(block&& other) noexcept;

	block& operator=(const block& other);

	block& operator=(block&& other) noexcept;

	~block() = default;

	[[nodiscard]] const compound_ptr& compound() const;
	[[nodiscard]] const var_decl_list& var_declarations() const;
	[[nodiscard]] const procedure_decl_list& procedure_declarations() const;
	
	void accept(ast_node_visitor& visitor) override;
};

