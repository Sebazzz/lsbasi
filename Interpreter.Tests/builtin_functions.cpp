#include "common_test.h"
#include "../Interpreter/interpreter.h"

TEST_CASE( "Interpretation succeeds - builtin function", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: REAL;
BEGIN       
   a := random();
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_real_symbol(result, L"a") != 0 /*Default value and very unlikely to be the outcome of random*/ );
}

