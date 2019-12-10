#include "common_test.h"


TEST_CASE( "Interpretation succeeds - call to builtin procedure", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
BEGIN       
   writeln('hello world')
END.        
)");

    REQUIRE( result.output == std::wstring(L"hello world\n") );
}

TEST_CASE( "Interpretation succeeds - call to builtin procedure with uninitialized variable", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Simple;
VAR a: STRING;
BEGIN       
   writeln(a)
END.        
)");

    REQUIRE( result.output == std::wstring(L"\n") );
}
