#include "common_test.h"
#include "interpreter_test_support.h"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/symbol.h"
#include "../Interpreter/type.h"
#include "../Interpreter/routine_symbol.h"
#include "../Interpreter/var_decl.h"
#include "../Interpreter/builtin_type_symbol.h"
#include "../Interpreter/symbol_table_builder.h"

builtin_boolean verify_bool_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    return verify_symbol<ast::builtin_type::boolean>(result, identifier);
}

builtin_integer verify_int_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    return verify_symbol<ast::builtin_type::integer>(result, identifier);
}

builtin_real verify_real_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    return verify_symbol<ast::builtin_type::real>(result, identifier);
}

builtin_string verify_string_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    return *verify_symbol<ast::builtin_type::string>(result, identifier);
}

interpret_result test_program_interpretation(const char* input)
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

std::wstring test_repl_interpretation(const std::wstring& input)
{
    std::wstringstream input_stream;
	input_stream.str(input);

    interpreter sut(input_stream, true);

    return sut.interpret();
}

parse_result test_symbol_table_builder(const char* input)
{
    std::wstringstream input_stream;
	input_stream.str(raw_to_wstring(input));

	const interpreter_context_ptr context = std::make_shared<interpreter_context>();
    parser sut(input_stream, context);

    const auto result = sut.parse();

    symbol_table_builder symbol_visitor;
    symbol_visitor.visit(*result);

    return {
        symbol_visitor.symbol_table(),
        result
    };
}
