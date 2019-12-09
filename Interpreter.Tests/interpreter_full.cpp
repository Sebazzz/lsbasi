#include "common_test.h"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/routine_symbol.h"

TEST_CASE( "Interpretation succeeds - program 1", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;  
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 2 );
}

TEST_CASE( "Interpretation succeeds - program 2", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR a, b, c: REAL;
    x, number: INTEGER;
BEGIN                                   
    BEGIN                               
        number := 2;                    
        a := number;                    
        b := 10 * a + 10 * number / 4;  
        c := a - - b                    
    END;                                
    x := 11;                            
END.                                    
)");

	REQUIRE(result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"x") == 11 );
    REQUIRE( verify_int_symbol(result, L"number") == 2 );

    REQUIRE( verify_real_symbol(result, L"a") == 2 );
    REQUIRE( verify_real_symbol(result, L"b") == 25 );
    REQUIRE( verify_real_symbol(result, L"c") == 27 );
}

TEST_CASE( "Interpretation succeeds - program (division using 'div' keyword)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;
BEGIN       
   _a := 2 div 3;  
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a") == 0 );
}
