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


TEST_CASE( "Interpretation succeeds - functions result as argument", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;

FUNCTION F1: INTEGER;
BEGIN {F1}
    F1 := 4 div 2
END;

FUNCTION F2(q : INTEGER): INTEGER;
BEGIN {F2}
    F2 := q * q
END;

BEGIN       
   _a := F2(F2(F2(F1())));  
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a") == 256 );
}



TEST_CASE( "Interpretation succeeds - functions has too many args", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Semi;                           
VAR _a: INTEGER;

FUNCTION F2(q : INTEGER): INTEGER;
BEGIN {F2}
    F2 := q * q
END;

BEGIN       
   _a := F2(4,5);  
END.        
)";

     REQUIRE_THROWS_MATCHES( 
		test_program_interpretation(program), 
		interpret_except,
		Catch::Message("In a call to function F2 too many arguments have been provided"));
}
