#include "catch.hpp"
#include "../Interpreter/parser.h"
#include "../Interpreter/rpn_visitor.h"

std::wstring do_rpn(std::wstring input)
{
    parser parser(std::move(input));
    const auto result = parser.parse_repl();

    rpn_visitor visitor;
    visitor.visit(*result);

    return visitor.get_string();
}

TEST_CASE( "Reverse polish notation visit", "[rpn_visitor]" ) {
    REQUIRE( do_rpn(L"3 + 4") == std::wstring(L"3 4 +") );
    REQUIRE( do_rpn(L"3 - 4 + 5") == std::wstring(L"3 4 - 5 +") );
    REQUIRE( do_rpn(L"(5 + 3) * 12 / 3") == std::wstring(L"5 3 + 12 * 3 /") );
}