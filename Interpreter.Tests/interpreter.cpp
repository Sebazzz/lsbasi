#include "catch.hpp"
#include "../Interpreter/interpreter.h"

std::wstring do_interpret(std::wstring input)
{
    interpreter sut(std::move(input));

    return sut.interpret();
}

TEST_CASE( "Interpretation succeeds", "[interpreter]" ) {
    REQUIRE( do_interpret(L"1+1") == std::wstring(L"2.000000") );
}