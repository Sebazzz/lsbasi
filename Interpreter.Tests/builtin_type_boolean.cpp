#include "common_test.h"


TEST_CASE( "Interpretation succeeds - program with boolean", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a, b: BOOLEAN;
BEGIN       
   a := true;
   b := false;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_bool_symbol(result, L"a") == true );
    REQUIRE( verify_bool_symbol(result, L"b") == false );
}

TEST_CASE( "Interpretation fails - program with boolean with operators", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR a: BOOLEAN;
BEGIN       
   a := true + false;
END.        
)";

	REQUIRE_THROWS_MATCHES( 
		test_program_interpretation(program), 
		exec_error,
		Catch::Message("Unable to execute binary operation: token(plus) - Binary operators are not supported for booleans"));
}