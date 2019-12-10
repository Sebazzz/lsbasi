#include "pch.h"
#include "routine_call.h"
#include "ast_node_visitor.h"

ast::routine_call::routine_call(ast::procedure_identifier procedure_identifier, procedure_arg_list shared_ptrs, token token): ast_node(std::move(token)),
	m_procedure_identifier(std::move(procedure_identifier)),
	m_args(std::move(shared_ptrs))
{
}

ast::routine_call::routine_call(const routine_call& other) = default;

ast::routine_call::routine_call(routine_call&& other) noexcept: ast_node(std::move(other)),
                                                                      m_procedure_identifier(
	                                                                      std::move(other.m_procedure_identifier)),
                                                                      m_args(std::move(other.m_args))
{
}

ast::routine_call& ast::routine_call::operator=(const routine_call& other)
{
	if (this == &other)
		return *this;
	ast_node::operator =(other);
	m_procedure_identifier = other.m_procedure_identifier;
	m_args = other.m_args;
	return *this;
}

ast::routine_call& ast::routine_call::operator=(routine_call&& other) noexcept
{
	if (this == &other)
		return *this;
	m_procedure_identifier = std::move(other.m_procedure_identifier);
	m_args = std::move(other.m_args);
	ast_node::operator =(std::move(other));
	return *this;
}

const ast::procedure_identifier& ast::routine_call::procedure_identifier() const
{
	return this->m_procedure_identifier;
}

const ast::procedure_arg_list& ast::routine_call::args() const
{
	return this->m_args;
}

void ast::routine_call::accept(ast_node_visitor& visitor)
{
	visitor.visit(*this);
}

symbol_type_ptr<routine_symbol> ast::routine_call::routine_symbol() const
{
	if (!this->m_routine_symbol)
	{
		throw internal_interpret_except(L"Procedure symbol was not made available in semantic analysis step");
	}
	
	return this->m_routine_symbol;
}
