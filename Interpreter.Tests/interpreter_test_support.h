#pragma once
#include "interpreter.h"
#include "builtin_type_traits.h"
#include "symbol.h"
#include "type.h"
#include "routine_symbol.h"
#include "var_decl.h"
#include "builtin_type_symbol.h"

struct interpret_result
{
    std::shared_ptr<scope_context> global_scope;
    interpreter_context_ptr context;
    ast_ptr ast;
    std::wstring output;
};

struct parse_result
{
    std::shared_ptr<symbol_table> symbol_table;
    ast::ast_ptr ast;
};

template<ast::builtin_type T>
typename builtin_type_traits<T>::builtin_type verify_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto var_symbol = result.global_scope->symbols.get<variable_symbol>(identifier);
    const auto& builtin_type = result.global_scope->symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());

    if (builtin_type->type() != T)
    {
        throw std::logic_error("ASSERT FAILED: Symbol was not an " + wstring_to_string(std::wstring(builtin_type_traits<T>::type_name)));
    }

    auto contents = result.global_scope->memory->get(var_symbol);
    return builtin_type_traits<T>::get_from_expression(contents);
}

builtin_boolean verify_bool_symbol(const interpret_result& result, const symbol_identifier& identifier);
builtin_integer verify_int_symbol(const interpret_result& result, const symbol_identifier& identifier);
builtin_real verify_real_symbol(const interpret_result& result, const symbol_identifier& identifier);
builtin_string verify_string_symbol(const interpret_result& result, const symbol_identifier& identifier);
interpret_result test_program_interpretation(const char* input);
std::wstring test_repl_interpretation(const std::wstring& input);
parse_result test_symbol_table_builder(const char* input);
