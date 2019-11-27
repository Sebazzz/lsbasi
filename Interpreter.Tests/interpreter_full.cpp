#include "catch.hpp"
#include "../Interpreter/interpreter.h"

std::wstring do_interpret_program(std::wstring input)
{
    interpreter sut(std::move(input), false);

    return sut.interpret();
}

TEST_CASE( "Interpretation succeeds - program", "[interpreter_program]" ) {
    REQUIRE( do_interpret_program(L"\
PROGRAM Simple;\
VAR a: INTEGER; b: REAL;\
BEGIN       \
   a := 2;  \
END.        \
") == std::wstring(L"done") );

	REQUIRE( do_interpret_program(L"\
PROGRAM Semi;                           \
VAR a, b, c, number: REAL;\
BEGIN                                   \
    BEGIN                               \
        number := 2;                    \
        a := number;                    \
        b := 10 * a + 10 * number / 4;  \
        c := a - - b                    \
    END;                                \
    x := 11;                            \
END.                                    \
") == std::wstring(L"done") );
}

TEST_CASE( "Interpretation succeeds - program (case insensitive)", "[interpreter_program]" ) {
	REQUIRE( do_interpret_program(L"\
PROGRAM Semi;                           \
begIN                                   \
    BegiN                               \
        NUMBer := 2;                    \
        a := numBER;                    \
        b := 10 * A + 10 * number / 4;  \
        c := A - - B                    \
    end;                                \
    X := 11;                            \
ENd.                                    \
") == std::wstring(L"done") );
}

TEST_CASE( "Interpretation succeeds - program (vars with underscore)", "[interpreter_program]" ) {
    REQUIRE( do_interpret_program(L"\
PROGRAM Semi;                           \
VAR _a: INTEGER;\
BEGIN       \
   _a := 2;  \
END.        \
") == std::wstring(L"done") );

    REQUIRE( do_interpret_program(L"\
PROGRAM Semi;                           \
VAR _a_b: INTEGER;\
BEGIN       \
   _a_b := 2;  \
END.        \
") == std::wstring(L"done") );
}

TEST_CASE( "Interpretation succeeds - program (division using 'div' keyword)", "[interpreter_program]" ) {
    REQUIRE( do_interpret_program(L"\
PROGRAM Semi;                           \
VAR _a: INTEGER;\
BEGIN       \
   _a := 2 div 3;  \
END.        \
") == std::wstring(L"done") );
}