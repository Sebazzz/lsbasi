#pragma once
#include "ast_common.h"

namespace ast
{
	using procedure_identifier = std::wstring;

	class block;
	class procedure;

	using block_ptr = ast_node_ptr<block>;
}


class ast::procedure :
	public ast_node
{
private:
	procedure_identifier m_identifier;
	block_ptr m_block;

public:
	procedure(procedure_identifier id, block_ptr block);

	procedure(const procedure& other) = default;

	procedure(procedure&& other) noexcept;

	procedure& operator=(const procedure& other);

	procedure& operator=(procedure&& other) noexcept;

	~procedure() = default;

	[[nodiscard]] const procedure_identifier& identifier() const;
	[[nodiscard]] const block_ptr& block() const;
	
	void accept(ast_node_visitor& visitor) override;
};

