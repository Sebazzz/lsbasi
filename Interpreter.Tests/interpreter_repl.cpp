#include "common_test.h"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/interpret_except.h"

std::wstring do_interpret(std::wstring input)
{
    interpreter sut(std::move(input), true);

    return sut.interpret();
}

TEST_CASE( "Interpretation succeeds - plus/minus", "[interpreter]" ) {
    REQUIRE( do_interpret(L"1+1") == std::wstring(L"2") );
    REQUIRE( do_interpret(L"1-1") == std::wstring(L"0") );
    REQUIRE( do_interpret(L"1--1") == std::wstring(L"2") );
    REQUIRE( do_interpret(L"5+-2") == std::wstring(L"3") );
}

TEST_CASE( "Interpretation succeeds - plus/minus - multi-digit", "[interpreter]" ) {
    REQUIRE( do_interpret(L"10+16") == std::wstring(L"26") );
    REQUIRE( do_interpret(L"10-15") == std::wstring(L"-5") );
   
    REQUIRE( do_interpret(L"50+-52") == std::wstring(L"-2") );
    REQUIRE( do_interpret(L"-50 div 2") == std::wstring(L"-25") );

    REQUIRE( do_interpret(L"10*31") == std::wstring(L"310") );
    REQUIRE( do_interpret(L"50 div 2") == std::wstring(L"25") );
    REQUIRE( do_interpret(L"1+2*3") == std::wstring(L"7") );
    REQUIRE( do_interpret(L"-5+5*3") == std::wstring(L"10") );
}

TEST_CASE( "Interpretation succeeds - multiply/divide", "[interpreter]" ) {
    REQUIRE( do_interpret(L"-50 div 2") == std::wstring(L"-25") );
    REQUIRE( do_interpret(L"-50/2") == std::wstring(L"-25.000000") );

    REQUIRE( do_interpret(L"10*31") == std::wstring(L"310") );
    REQUIRE( do_interpret(L"50 div 2") == std::wstring(L"25") );
    REQUIRE( do_interpret(L"50 / 2") == std::wstring(L"25.000000") );
}

TEST_CASE( "Interpretation succeeds - mixed multiple terms", "[interpreter]" ) {
    REQUIRE( do_interpret(L"1+2*3") == std::wstring(L"7") );
    REQUIRE( do_interpret(L"-5+5*3") == std::wstring(L"10") );
}

TEST_CASE( "Interpretation succeeds - unary operator support", "[interpreter]" ) {
    REQUIRE( do_interpret(L"-1") == std::wstring(L"-1") );
    REQUIRE( do_interpret(L"-(-1)") == std::wstring(L"1") );
    REQUIRE( do_interpret(L"-(-(-1))") == std::wstring(L"-1") );
    REQUIRE( do_interpret(L"-(2+3)") == std::wstring(L"-5") );
}

TEST_CASE( "Interpretation succeeds - groups", "[interpreter]" ) {
    REQUIRE( do_interpret(L"(1+2)-1") == std::wstring(L"2") );
    REQUIRE( do_interpret(L"1+((1+2)-1)") == std::wstring(L"3") );
    REQUIRE( do_interpret(L"(1+2)*3") == std::wstring(L"9") );
    REQUIRE( do_interpret(L"(-5+5)*3") == std::wstring(L"0") );
    REQUIRE( do_interpret(L"((-5+1)*5)*3") == std::wstring(L"-60") );
    REQUIRE( do_interpret(L"7 + 3 * (10 / (12 / (3 + 1) - 1))") == std::wstring(L"22.000000") );
    REQUIRE( do_interpret(L"7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)") == std::wstring(L"10.000000") );
}

TEST_CASE( "Interpretation fails on bad syntax", "[interpreter]" ) {
    REQUIRE_THROWS_MATCHES( do_interpret(L"50 50"), interpret_except, Catch::Message("Unexpected token found: token(int,50)"));
    //REQUIRE_THROWS_MATCHES( do_interpret(L"50 ++ 50"), interpret_except, Catch::Message("Expected integer"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"+"), interpret_except, Catch::Message("Found end while searching for factor"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"-"), interpret_except, Catch::Message("Found end while searching for factor"));
    REQUIRE_THROWS_MATCHES( do_interpret(L""), interpret_except, Catch::Message("string is empty"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"5--"), interpret_except, Catch::Message("Found end while searching for factor"));
}

TEST_CASE( "Interpretation fails on bad syntax - groups", "[interpreter]" ) {
    REQUIRE_THROWS_MATCHES( do_interpret(L"1+(1+2)-1)") == std::wstring(L"3"), interpret_except, Catch::Message("Unexpected token found: token(group_end)"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"50 + 50 )"), interpret_except, Catch::Message("Unexpected token found: token(group_end)"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"(50 + 50))"), interpret_except, Catch::Message("Unexpected token found: token(group_end)"));
    REQUIRE_THROWS_MATCHES( do_interpret(L"((50+50)"), interpret_except, Catch::Message("At the end of the file while expecting token: group_end"));
}