#pragma once
#include <utility>
#include "ast_node.h"
#include "token_type.h"

class bin_op final : public ast_node
{
private:
	ast_ptr m_left;
	token_type op;
	ast_ptr m_right;

public:
	bin_op(ast_ptr left, token_type op, ast_ptr right)
		: ast_node(op),
		  m_left(std::move(left)),
		  op(op),
		  m_right(std::move(right))
	{
	}


	bin_op(const bin_op& other) = default;

	bin_op(bin_op&& other) noexcept
		: ast_node(std::move(other)),
		  m_left(std::move(other.m_left)),
		  op(other.op),
		  m_right(std::move(other.m_right))
	{
	}

	bin_op& operator=(const bin_op& other)
	{
		if (this == &other)
			return *this;
		ast_node::operator =(other);
		m_left = other.m_left;
		op = other.op;
		m_right = other.m_right;
		return *this;
	}

	bin_op& operator=(bin_op&& other) noexcept
	{
		if (this == &other)
			return *this;
		m_left = std::move(other.m_left);
		op = other.op;
		m_right = std::move(other.m_right);
		
		ast_node::operator =(std::move(other));
		return *this;
	}

	~bin_op() = default;

	[[nodiscard]] ast_ptr left() const
	{
		return m_left;
	}

	[[nodiscard]] token_type op1() const
	{
		return op;
	}

	[[nodiscard]] ast_ptr right() const
	{
		return m_right;
	}
};

