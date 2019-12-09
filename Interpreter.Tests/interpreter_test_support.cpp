#include "common_test.h"
#include "interpreter_test_support.h"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/symbol.h"
#include "../Interpreter/type.h"
#include "../Interpreter/routine_symbol.h"
#include "../Interpreter/var_decl.h"
#include "../Interpreter/builtin_type_symbol.h"

builtin_boolean verify_bool_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto var_symbol = result.global_scope->symbols.get<variable_symbol>(identifier);
    const auto& contents = result.global_scope->memory->get(var_symbol);
    const auto& builtin_type = result.global_scope->symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());

    if (builtin_type->type() != ast::builtin_type::boolean)
    {
        throw std::logic_error("Symbol was not an integer");
    }

    return contents.boolean_val;
}

builtin_integer verify_int_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto var_symbol = result.global_scope->symbols.get<variable_symbol>(identifier);
    const auto& contents = result.global_scope->memory->get(var_symbol);
    const auto& builtin_type = result.global_scope->symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());

    if (builtin_type->type() != ast::builtin_type::integer)
    {
        throw std::logic_error("Symbol was not an integer");
    }

    return contents.int_val;
}

builtin_real verify_real_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto var_symbol = result.global_scope->symbols.get<variable_symbol>(identifier);
    const auto& contents = result.global_scope->memory->get(var_symbol);
    const auto& builtin_type = result.global_scope->symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());

    if (builtin_type->type() != ast::builtin_type::real)
    {
        throw std::logic_error("Symbol was not an real");
    }

    return contents.real_val;
}

builtin_string verify_string_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto var_symbol = result.global_scope->symbols.get<variable_symbol>(identifier);
    const auto& contents = result.global_scope->memory->get(var_symbol);
    const auto& builtin_type = result.global_scope->symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());

    if (builtin_type->type() != ast::builtin_type::string)
    {
        throw std::logic_error("Symbol was not an string");
    }

    return *contents.string_ptr_val;
}

interpret_result do_interpret_program(const char* input)
{
    // Replace the stdout so we can capture the interpreter output
    const auto old_wcout_buf = std::wcout.rdbuf();
    std::wstringstream wcout_stream;
    std::wcout.rdbuf(wcout_stream.rdbuf());

    // String-stream for interpreter input
    std::wstringstream input_stream;
	input_stream.str(raw_to_wstring(input));

    interpreter sut(input_stream, false);

    // Collect results
    std::wcout << sut.interpret();
    const auto inter_info = sut.get_interpretation_info();

    // Restore std::wcout stream
    std::wcout.rdbuf(old_wcout_buf);

    // ... Capture output
    wcout_stream.seekg(0);
    std::wstring interpret_output = wcout_stream.str();
    

    return {
        inter_info.global_scope,
        inter_info.interpreter_context,
        inter_info.ast,
        interpret_output
    };
}