#pragma once
#include "../Interpreter/interpreter.h"
#include "../Interpreter/symbol.h"
#include "../Interpreter/type.h"
#include "../Interpreter/routine_symbol.h"
#include "../Interpreter/var_decl.h"
#include "../Interpreter/builtin_type_symbol.h"

struct interpret_result
{
    std::shared_ptr<scope_context> global_scope;
    interpreter_context_ptr context;
    ast_ptr ast;
    std::wstring output;
};


builtin_boolean verify_bool_symbol(const interpret_result& result, const symbol_identifier& identifier);
builtin_integer verify_int_symbol(const interpret_result& result, const symbol_identifier& identifier);
builtin_real verify_real_symbol(const interpret_result& result, const symbol_identifier& identifier);
builtin_string verify_string_symbol(const interpret_result& result, const symbol_identifier& identifier);
interpret_result do_interpret_program(const char* input);
