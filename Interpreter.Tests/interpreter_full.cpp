#include "catch.hpp"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/interpret_except.h"

std::wstring do_interpret_program(std::wstring input)
{
    interpreter sut(std::move(input), false);

    return sut.interpret();
}

TEST_CASE( "Interpretation succeeds - program", "[interpreter_program]" ) {
    REQUIRE( do_interpret_program(L"\
BEGIN       \
   a := 2;  \
END.        \
") == std::wstring(L"done") );

	REQUIRE( do_interpret_program(L"\
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