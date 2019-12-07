#include "pch.h"
#include "assignment_target.h"
#include "ast_node_visitor.h"
#include "symbol.h"
#include "routine_symbol.h"

using namespace ast;

assignment_target::assignment_target(var_identifier identifier, token token): ast_node(std::move(token)), m_identifier(std::move(identifier))
{
}

assignment_target::assignment_target(assignment_target&& other) noexcept: ast_node(other),
                                m_identifier(other.m_identifier)
{
}

assignment_target& assignment_target::operator=(const assignment_target& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_identifier = other.m_identifier;
	return *this;
}

assignment_target& assignment_target::operator=(assignment_target&& other) noexcept
{
	if (this == &other)
		return *this;
	m_identifier = other.m_identifier;
	ast_node::operator =(other);
	return *this;
}

const var_identifier& assignment_target::identifier() const
{
	return m_identifier;
}

symbol_type_ptr<type_symbol> assignment_target::type_symbol() const
{
	if (this->m_function_symbol)
	{
		return this->m_function_symbol->return_type();
	}

	if (this->m_variable_symbol)
	{
		return this->m_variable_symbol->variable().type()->type_symbol();
	}

	throw internal_interpret_except(L"Unable to determine type symbol for assignment target. It should have been assigned in the semantic analysis step.");
}

symbol_ptr assignment_target::assignment_symbol() const
{
	if (this->m_variable_symbol)
	{
		return std::static_pointer_cast<symbol, variable_symbol>(this->m_variable_symbol);
	}

	if (this->m_function_symbol)
	{
		return std::static_pointer_cast<symbol, routine_symbol>(this->m_function_symbol);
	}

	throw internal_interpret_except(L"Variable or function symbol was not made available in semantic analysis step");
}

void assignment_target::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}