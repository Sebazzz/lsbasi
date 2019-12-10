#include "common_test.h"
#include "../Interpreter/interpreter.h"

TEST_CASE( "Conditionals: a = 2; a == 2", "[conditionals]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; result: BOOLEAN;
BEGIN       
   a := 2;
   result := a = 2;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 2 );
    REQUIRE( verify_bool_symbol(result, L"result") == true );
}

TEST_CASE( "Conditionals: a = 4; a == 2", "[conditionals]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; result: BOOLEAN;
BEGIN       
   a := 4;
   result := a = 2;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 4 );
    REQUIRE( verify_bool_symbol(result, L"result") == false );
}

