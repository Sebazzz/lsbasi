#pragma once
#include "ast_node.h"
#include "token_type.h"
#include <utility>
#include <vector>

namespace ast
{
	class compound;

	using statement_list = std::vector<ast_ptr>;
}

class ast::compound final : public ast_node
{
private:
	statement_list m_statements;

public:

	compound(ast::statement_list ast_nodes);

	compound(const compound& other);

	compound(compound&& other) noexcept;

	compound& operator=(const compound& other);

	compound& operator=(compound&& other) noexcept;

	~compound() = default;

	const statement_list& statements() const;
	
	void accept(ast_node_visitor& visitor) override;
};

