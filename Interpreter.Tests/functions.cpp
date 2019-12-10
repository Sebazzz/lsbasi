#include "common_test.h"

#include "routine_symbol.h"

TEST_CASE( "Interpretation succeeds - functions", "[functions]" ) {
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


TEST_CASE( "Interpretation succeeds - functions result as argument", "[functions]" ) {
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



TEST_CASE( "Interpretation succeeds - functions has too many args", "[functions]" ) {
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


TEST_CASE( "Interpretation succeeds - recursion", "[functions]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Recursion;
VAR result: INTEGER;

FUNCTION Recursive(n: INTEGER): INTEGER;
BEGIN
	IF n = 0 THEN
		Recursive := 10
	ELSE
		Recursive := Recursive(n-1)
END;

BEGIN
   result := Recursive(1)
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"result") == 10 );
}
