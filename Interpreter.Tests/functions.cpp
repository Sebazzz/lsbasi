#include "common_test.h"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/routine_symbol.h"

TEST_CASE( "Interpretation succeeds - functions", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;

FUNCTION F1: INTEGER;
BEGIN {F1}
    F1 := 4 div 2
END;

BEGIN       
   _a := F1();  
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a") == 2 );
}