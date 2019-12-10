#include "common_test.h"

#include "routine_symbol.h"


TEST_CASE( "Implicit assignment from real to integer disallowed", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR a: INTEGER;
    b: REAL;
BEGIN       
   a := b;  
END.        
)";

    REQUIRE_THROWS_MATCHES(
		test_program_interpretation(program),
		runtime_type_error, 
		Catch::Message("Runtime type error: Attempting to assign variable variable a (which is of type built-in INTEGER) from expression with invalid type: built-in REAL"));
}

TEST_CASE( "Implicit assignment from integer to real allowed", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;  
   b := a;  
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 2 );
    REQUIRE( verify_real_symbol(result, L"b") == 2 );
}

TEST_CASE( "Assignment from string to real disallowed", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR str: STRING; b: REAL;
BEGIN       
   str := 'HELLO WORLD';  
   b := str;  
END.        
)";

        REQUIRE_THROWS_MATCHES( test_program_interpretation(program), 
			runtime_type_error, 
			Catch::Message("Runtime type error: Attempting to assign variable variable b (which is of type built-in REAL) from expression with invalid type: built-in STRING"));

}

TEST_CASE( "Assignment from string to integer disallowed", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR str: STRING; b: INTEGER;
BEGIN       
   str := 'HELLO WORLD';  
   b := str;  
END.        
)";

        REQUIRE_THROWS_MATCHES( test_program_interpretation(program), 
			runtime_type_error, 
			Catch::Message("Runtime type error: Attempting to assign variable variable b (which is of type built-in INTEGER) from expression with invalid type: built-in STRING"));

}