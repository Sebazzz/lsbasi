#include "common_test.h"
#include "../Interpreter/interpreter.h"

TEST_CASE( "Interpretation succeeds - program with if/then", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER;
BEGIN       
   a := 2;
   IF a = 2 THEN
	  a := 4
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 4 );
}

TEST_CASE( "Interpretation succeeds - program with if/then (multiline)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER;
BEGIN       
   a := 2;
   IF a = 2 THEN
   BEGIN
	  a := 4
   END
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 4 );
}

TEST_CASE( "Interpretation succeeds - program with if/then (no match)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER;
BEGIN       
   a := 2;
   IF a = 3 THEN
	  a := 4.6 { This should not execute }
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 2 );
}

TEST_CASE( "Interpretation succeeds - program with if/then (no match, multiline)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER;
BEGIN       
   a := 2;
   IF a = 3 THEN
   BEGIN
	  a := 4.6 { This should not execute }
   END
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 2 );
}

TEST_CASE( "Interpretation succeeds - program with if/then/else", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;
   IF a = 2 THEN
	  a := 4
   ELSE
      a := -2
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 4 );
}

TEST_CASE( "Interpretation succeeds - program with if/then/else (multiline)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;
   IF a = 2 THEN
   BEGIN
      a := 4
   END
   ELSE
   BEGIN
      a := -2
   END
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 4 );
}

TEST_CASE( "Interpretation succeeds - program with if/then/else (multiline - mixed)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;
   IF a = 2 THEN
   BEGIN
      a := 4
   END
   ELSE
      a := -2
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 4 );
}

TEST_CASE( "Interpretation succeeds - program with if/then/else (no match)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;
   IF a = 3 THEN
	  a := 4
   ELSE
      a := -2
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == -2 );
}

TEST_CASE( "Interpretation succeeds - program with if/then/else (no match, multiline)", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;
   IF a = 3 THEN
   BEGIN
	  a := 4
   END
   ELSE
   BEGIN
      a := -2
   END
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == -2 );
}
