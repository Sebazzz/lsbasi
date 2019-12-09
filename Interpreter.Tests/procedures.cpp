#include "common_test.h"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/routine_symbol.h"


TEST_CASE( "Interpretation succeeds - procedures", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;

PROCEDURE P1;
BEGIN {P1}

END;

BEGIN       
   _a := 4 div 2;  
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a") == 2 );
}

TEST_CASE( "Interpretation succeeds - procedures with parameters", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
PROCEDURE P1(a : INTEGER);
BEGIN {P1}
    a:=2
END;

BEGIN       
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( (!!result.global_scope->symbols.get<routine_symbol>(L"P1")) == true );
}

TEST_CASE( "Interpretation succeeds - procedures with parameters called", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR a_global: INTEGER;
PROCEDURE P1(a : INTEGER);
BEGIN {P1}
    a_global:=a
END;

BEGIN       
    P1(5)
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( (!!result.global_scope->symbols.get<routine_symbol>(L"P1")) == true );
    REQUIRE( verify_int_symbol(result, L"a_global") == 5 );
}

TEST_CASE( "Interpretation succeeds - procedures with parameters called with expression", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Semi;                           
VAR a_global: INTEGER;
PROCEDURE P1(a : INTEGER);
BEGIN {P1}
    a_global:=a
END;

BEGIN       
    P1(4*5)
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( (!!result.global_scope->symbols.get<routine_symbol>(L"P1")) == true );
    REQUIRE( verify_int_symbol(result, L"a_global") == 20 );
}

TEST_CASE( "Interpretation succeeds - procedure variable hides global variable and sets global variable", "[interpreter_program]" ) {
    const auto result = test_program_interpretation(R"(
PROGRAM Part12;
VAR
   a, c : REAL; b : INTEGER;

PROCEDURE P1(a : REAL);
BEGIN {P1}
    c := a;
END;  {P1}

BEGIN {Part12}
   a := 10;
   b := 1337;
   P1(b);
END.  {Part12}
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_real_symbol(result, L"a") == 10 );
    REQUIRE( verify_real_symbol(result, L"c") == 1337 );
    REQUIRE( verify_int_symbol(result, L"b") == 1337 );
}

TEST_CASE( "Interpretation fails - procedures with parameters called with too many parameters", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Semi;                           
VAR a_global: INTEGER;
PROCEDURE P1(a : INTEGER);
BEGIN {P1}
    a_global:=a
END;

BEGIN       
    P1(5, 4)
END.        
)";

    REQUIRE_THROWS_MATCHES( 
		test_program_interpretation(program), 
		interpret_except, 
		Catch::Message("In a call to procedure P1 too many arguments have been provided"));
}

TEST_CASE( "Interpretation fails - procedure used in assignment", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Semi;                           
VAR a_global: INTEGER;
PROCEDURE P1(a : INTEGER);
BEGIN {P1}
    a_global:=a
END;

BEGIN       
    a_global:= P1(5);
END.        
)";

    // This error message is quite bad, but logical as it happens in the parsing phase.
    // The grammar currently does not support functions, so we don't allow it as part of an expression.
    REQUIRE_THROWS_MATCHES( 
		test_program_interpretation(program), 
		internal_interpret_except,  // TODO: improve this error message, perhaps we should be able to fix this in the semantic analysis step
		Catch::Message("INTERNAL INTERPRETER ERROR: Previous node accept did not yield a value: class ast::routine_call"));
}

TEST_CASE( "Interpretation fails - procedure used as argument", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Semi;                           
VAR a_global: INTEGER;
PROCEDURE P1(a : INTEGER);
BEGIN
    a_global:=a
END;

PROCEDURE P2(a : INTEGER);
BEGIN
    a_global:=a
END;

BEGIN       
    a_global:= P1(P2(5));
END.        
)";

    // This error message is quite bad, but logical as it happens in the parsing phase.
    // The grammar currently does not support functions, so we don't allow it as part of an expression.
    REQUIRE_THROWS_MATCHES( 
		test_program_interpretation(program), 
		internal_interpret_except,  // TODO: improve this error message, perhaps we should be able to fix this in the semantic analysis step
		Catch::Message("INTERNAL INTERPRETER ERROR: Previous node accept did not yield a value: class ast::routine_call"));
}

TEST_CASE( "Interpretation fails - procedures with parameters called with not enough parameters", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Semi;                           
VAR a_global: INTEGER;
PROCEDURE P1(a : INTEGER);
BEGIN {P1}
    a_global:=a
END;

BEGIN       
    P1()
END.        
)";

    REQUIRE_THROWS_MATCHES( 
		test_program_interpretation(program), 
		interpret_except, 
		Catch::Message("In a call to procedure P1 no argument has been provided for this parameter: a"));
}