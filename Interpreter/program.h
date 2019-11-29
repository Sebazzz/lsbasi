#pragma once
#include "ast_common.h"

namespace ast
{
	using program_identifier = std::wstring;

	class block;
	class program;

	using block_ptr = ast_node_ptr<block>;
}

class symbol_reference_visitor;
class symbol_table;

class ast::program :
	public ast_node
{
private:
	program_identifier m_identifier;
	block_ptr m_block;

	std::shared_ptr<symbol_table> m_symbol_table;
	
public:
	program(program_identifier id, block_ptr block);

	program(const program& other) = default;

	program(program&& other) noexcept;

	program& operator=(const program& other);

	program& operator=(program&& other) noexcept;

	~program() = default;

	[[nodiscard]] const program_identifier& identifier() const;
	[[nodiscard]] const block_ptr& block() const;
	
	void accept(ast_node_visitor& visitor) override;

	[[nodiscard]] symbol_table& symbol_table() const;

	friend class ::symbol_reference_visitor;
};

