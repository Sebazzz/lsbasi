#include "common_test.h"


TEST_CASE( "Interpretation succeeds - program with string", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a, b: STRING;
BEGIN       
   a := 'hello world';
   b := a;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_string_symbol(result, L"a") == L"hello world" );
    REQUIRE( verify_string_symbol(result, L"b") == L"hello world" );
}


TEST_CASE( "Interpretation succeeds - program with string concatenation", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a, b, c: STRING;
BEGIN       
   a := 'hello';
   b := 'world';
   c := a + ' ' + b;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_string_symbol(result, L"a") == L"hello" );
    REQUIRE( verify_string_symbol(result, L"b") == L"world" );
    REQUIRE( verify_string_symbol(result, L"c") == L"hello world" );
}

TEST_CASE( "Interpretation succeeds - program with string concatenation and uninitialized variable", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a, b, c: STRING;
BEGIN       
   a := 'hello';
   c := a + ' ' + b;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_string_symbol(result, L"a") == L"hello" );
    REQUIRE( verify_string_symbol(result, L"c") == L"hello " );
}