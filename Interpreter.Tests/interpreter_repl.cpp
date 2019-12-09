#include "common_test.h"
#include "interpreter_test_support.h"

TEST_CASE( "Interpretation succeeds - plus/minus", "[interpreter]" ) {
    REQUIRE( test_repl_interpretation(L"1+1") == std::wstring(L"2") );
    REQUIRE( test_repl_interpretation(L"1-1") == std::wstring(L"0") );
    REQUIRE( test_repl_interpretation(L"1--1") == std::wstring(L"2") );
    REQUIRE( test_repl_interpretation(L"5+-2") == std::wstring(L"3") );
}

TEST_CASE( "Interpretation succeeds - plus/minus - multi-digit", "[interpreter]" ) {
    REQUIRE( test_repl_interpretation(L"10+16") == std::wstring(L"26") );
    REQUIRE( test_repl_interpretation(L"10-15") == std::wstring(L"-5") );
   
    REQUIRE( test_repl_interpretation(L"50+-52") == std::wstring(L"-2") );
    REQUIRE( test_repl_interpretation(L"-50 div 2") == std::wstring(L"-25") );

    REQUIRE( test_repl_interpretation(L"10*31") == std::wstring(L"310") );
    REQUIRE( test_repl_interpretation(L"50 div 2") == std::wstring(L"25") );
    REQUIRE( test_repl_interpretation(L"1+2*3") == std::wstring(L"7") );
    REQUIRE( test_repl_interpretation(L"-5+5*3") == std::wstring(L"10") );
}

TEST_CASE( "Interpretation succeeds - multiply/divide", "[interpreter]" ) {
    REQUIRE( test_repl_interpretation(L"-50 div 2") == std::wstring(L"-25") );
    REQUIRE( test_repl_interpretation(L"-50/2") == std::wstring(L"-25.000000") );

    REQUIRE( test_repl_interpretation(L"10*31") == std::wstring(L"310") );
    REQUIRE( test_repl_interpretation(L"50 div 2") == std::wstring(L"25") );
    REQUIRE( test_repl_interpretation(L"50 / 2") == std::wstring(L"25.000000") );
}

TEST_CASE( "Interpretation succeeds - mixed multiple terms", "[interpreter]" ) {
    REQUIRE( test_repl_interpretation(L"1+2*3") == std::wstring(L"7") );
    REQUIRE( test_repl_interpretation(L"-5+5*3") == std::wstring(L"10") );
}

TEST_CASE( "Interpretation succeeds - unary operator support", "[interpreter]" ) {
    REQUIRE( test_repl_interpretation(L"-1") == std::wstring(L"-1") );
    REQUIRE( test_repl_interpretation(L"-(-1)") == std::wstring(L"1") );
    REQUIRE( test_repl_interpretation(L"-(-(-1))") == std::wstring(L"-1") );
    REQUIRE( test_repl_interpretation(L"-(2+3)") == std::wstring(L"-5") );
}

TEST_CASE( "Interpretation succeeds - groups", "[interpreter]" ) {
    REQUIRE( test_repl_interpretation(L"(1+2)-1") == std::wstring(L"2") );
    REQUIRE( test_repl_interpretation(L"1+((1+2)-1)") == std::wstring(L"3") );
    REQUIRE( test_repl_interpretation(L"(1+2)*3") == std::wstring(L"9") );
    REQUIRE( test_repl_interpretation(L"(-5+5)*3") == std::wstring(L"0") );
    REQUIRE( test_repl_interpretation(L"((-5+1)*5)*3") == std::wstring(L"-60") );
    REQUIRE( test_repl_interpretation(L"7 + 3 * (10 / (12 / (3 + 1) - 1))") == std::wstring(L"22.000000") );
    REQUIRE( test_repl_interpretation(L"7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)") == std::wstring(L"10.000000") );
}

TEST_CASE( "Interpretation fails on bad syntax", "[interpreter]" ) {
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"50 50"), parse_except, Catch::Message("Syntax error: Unexpected token found: token(int,50)"));
    //REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"50 ++ 50"), interpret_except, Catch::Message("Expected integer"));
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"+"), parse_except, Catch::Message("Syntax error: Found end while searching for factor"));
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"-"), parse_except, Catch::Message("Syntax error: Found end while searching for factor"));
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L""), parse_except, Catch::Message("Syntax error: string is empty"));
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"5--"), parse_except, Catch::Message("Syntax error: Found end while searching for factor"));
}

TEST_CASE( "Interpretation fails on bad syntax - groups", "[interpreter]" ) {
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"1+(1+2)-1)") == std::wstring(L"3"), parse_except, Catch::Message("Syntax error: Unexpected token found: token(group_end)"));
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"50 + 50 )"), parse_except, Catch::Message("Syntax error: Unexpected token found: token(group_end)"));
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"(50 + 50))"), parse_except, Catch::Message("Syntax error: Unexpected token found: token(group_end)"));
    REQUIRE_THROWS_MATCHES( test_repl_interpretation(L"((50+50)"), parse_except, Catch::Message("Syntax error: At the end of the file while expecting token: group_end"));
}