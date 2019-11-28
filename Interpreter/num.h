#pragma once
#include "var_decl.h"
#include "ast_common.h"

namespace ast
{
	class num;	
}

struct symbol_contents;

class ast::num :
	public ast_node
{
private:
	symbol_value m_value;
	var_type m_type;

public:
	num(double value);
	num(int value);

	num(const num& other) = default;

	num(num&& other) noexcept;

	num& operator=(const num& other);

	num& operator=(num&& other) noexcept;

	~num() = default;

	[[nodiscard]] std::wstring val_to_string() const;
	[[nodiscard]] symbol_contents to_symbol_contents() const;
	[[nodiscard]] var_type type() const;
	
	void accept(ast_node_visitor& visitor) override;
};

