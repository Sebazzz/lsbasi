#include "common_test.h"


TEST_CASE( "Interpretation succeeds - program (case insensitive)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR a, b, c: REAL;
    x, number: INTEGER;
begIN                                   
    BegiN                               
        NUMBer := 2;                    
        a := numBER;                    
        b := 10 * A + 10 * number / 4;  
        c := A - - B                    
    end;                                
    X := 11;                            
ENd.                                    
)");

	REQUIRE(result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"x") == 11 );
    REQUIRE( verify_int_symbol(result, L"number") == 2 );

    REQUIRE( verify_real_symbol(result, L"a") == 2 );
    REQUIRE( verify_real_symbol(result, L"b") == 25 );
    REQUIRE( verify_real_symbol(result, L"c") == 27 );
}

TEST_CASE( "Interpretation succeeds - program (vars with underscore) 1", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;
BEGIN       
   _a := 2;  
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a") == 2 );
}

TEST_CASE( "Interpretation succeeds - program (vars with underscore) 2", "[interpreter_program]" ) {
     const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR _a_b: INTEGER;
BEGIN       
   _a_b := 2;  
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a_b") == 2 );
}