#include "common_test.h"
#include "../Interpreter/interpreter.h"

TEST_CASE( "Interpretation succeeds - program fibonacci", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Fibonacci;
VAR result: INTEGER;

FUNCTION F(n: INTEGER): INTEGER;
BEGIN
	IF n = 0 THEN
		F := 0
	ELSE
		IF n = 1 THEN
			F := 1
		ELSE
			F := F(n-1) + F(n-2)
END;

BEGIN
   result := F(12)
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"result") == 144 );
}