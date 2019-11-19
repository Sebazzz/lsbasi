#include "catch.hpp"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/interpret_except.h"

std::wstring do_interpret(std::wstring input)
{
    interpreter sut(std::move(input));

    return sut.interpret();
}

TEST_CASE( "Interpretation succeeds", "[interpreter]" ) {
    REQUIRE( do_interpret(L"1+1") == std::wstring(L"2") );
    REQUIRE( do_interpret(L"1-1") == std::wstring(L"0") );
    REQUIRE( do_interpret(L"10*31") == std::wstring(L"310") );
    REQUIRE( do_interpret(L"50/2") == std::wstring(L"25") );
    REQUIRE( do_interpret(L"50+-52") == std::wstring(L"-2") );
    REQUIRE( do_interpret(L"-50/2") == std::wstring(L"-25") );
}

TEST_CASE( "Interpretation fails on bad syntax", "[interpreter]" ) {
    REQUIRE_THROWS_MATCHES( do_interpret(L"50 50"), interpret_except, Catch::Message("Expected operator but found different token instead"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"50 ++ 50"), interpret_except, Catch::Message("Expected integer"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"+"), interpret_except, Catch::Message("Expected integer"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"-"), interpret_except, Catch::Message("Expected integer"));
    REQUIRE_THROWS_MATCHES( do_interpret(L""), interpret_except, Catch::Message("string is empty"));
}