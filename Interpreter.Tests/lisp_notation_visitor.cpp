#include "common_test.h"
#include "parser.h"
#include "lisp_notation_visitor.h"

std::wstring do_lsp(const std::wstring& input)
{
    std::wstringstream input_stream;
	input_stream.str(input);

	const interpreter_context_ptr context = std::make_shared<interpreter_context>();
    parser parser(input_stream, context);
    const auto result = parser.parse_repl();

    lisp_notation_visitor visitor;
    visitor.visit(*result);

    return visitor.get_string();
}

TEST_CASE( "Lisp notation visitor", "[lisp_notation_visitor]" ) {
    REQUIRE( do_lsp(L"2 + 3") == std::wstring(L"(+ 2 3)") );
    REQUIRE( do_lsp(L"(2 + 3 * 5)") == std::wstring(L"(+ 2 (* 3 5))") );
}