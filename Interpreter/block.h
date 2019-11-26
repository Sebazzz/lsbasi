#pragma once
#include "ast_node.h"
#include <vector>

namespace ast
{
	class block;
	class compound;
	class var_decl;

	using compound_ptr = ast_node_ptr<compound>;

	using var_decl_ptr = ast_node_ptr<var_decl>;
	using var_decl_list = std::vector<var_decl_ptr>;
}

class ast::block final : public ast_node
{
private:
	compound_ptr m_compound;
	var_decl_list m_var_declarations;

public:
	block(var_decl_list var_declarations, compound_ptr compound);

	block(const block& other) = default;

	block(block&& other) noexcept;

	block& operator=(const block& other);

	block& operator=(block&& other) noexcept;

	~block() = default;

	[[nodiscard]] const compound_ptr& compound() const;
	[[nodiscard]] const var_decl_list& var_declarations() const;
	
	void accept(ast_node_visitor& visitor) override;
};

