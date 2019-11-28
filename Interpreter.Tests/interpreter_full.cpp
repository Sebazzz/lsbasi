#include "catch.hpp"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/symbol.h"

struct interpret_result
{
    scope_context global_scope;
    std::wstring output;
};

int verify_int_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto& contents = result.global_scope.memory->get(identifier);

    if (contents.type != ast::var_type::integer)
    {
        throw std::logic_error("Symbol was not an integer");
    }

    return contents.value.int_val;
}

double verify_real_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto& contents = result.global_scope.memory->get(identifier);

    if (contents.type != ast::var_type::real)
    {
        throw std::logic_error("Symbol was not an real");
    }

    return contents.value.real_val;
}

interpret_result do_interpret_program(std::wstring input)
{
    interpreter sut(std::move(input), false);

    const auto result = sut.interpret();

    return {
        sut.global_scope(),
        result
    };
}

TEST_CASE( "Interpretation succeeds - program 1", "[interpreter_program]" ) {
    const auto result = do_interpret_program(L"\
PROGRAM Simple;\
VAR a: INTEGER; b: REAL;\
BEGIN       \
   a := 2;  \
END.        \
");

    REQUIRE( result.output == std::wstring(L"done") );

    REQUIRE( verify_int_symbol(result, L"a") == 2 );
}


TEST_CASE( "Interpretation succeeds - program 2", "[interpreter_program]" ) {
    const auto result = do_interpret_program(L"\
PROGRAM Semi;                           \
VAR a, b, c: REAL;\
    x, number: INTEGER;\
BEGIN                                   \
    BEGIN                               \
        number := 2;                    \
        a := number;                    \
        b := 10 * a + 10 * number / 4;  \
        c := a - - b                    \
    END;                                \
    x := 11;                            \
END.                                    \
");

	REQUIRE(result.output  == std::wstring(L"done") );

    REQUIRE( verify_int_symbol(result, L"x") == 11 );
    REQUIRE( verify_int_symbol(result, L"number") == 2 );

    REQUIRE( verify_real_symbol(result, L"a") == 2 );
    REQUIRE( verify_real_symbol(result, L"b") == 25 );
    REQUIRE( verify_real_symbol(result, L"c") == 27 );
}

TEST_CASE( "Interpretation succeeds - program (case insensitive)", "[interpreter_program]" ) {
    const auto result = do_interpret_program(L"\
PROGRAM Semi;                           \
VAR a, b, c: REAL;\
    x, number: INTEGER;\
begIN                                   \
    BegiN                               \
        NUMBer := 2;                    \
        a := numBER;                    \
        b := 10 * A + 10 * number / 4;  \
        c := A - - B                    \
    end;                                \
    X := 11;                            \
ENd.                                    \
");

	REQUIRE(result.output  == std::wstring(L"done") );

    REQUIRE( verify_int_symbol(result, L"x") == 11 );
    REQUIRE( verify_int_symbol(result, L"number") == 2 );

    REQUIRE( verify_real_symbol(result, L"a") == 2 );
    REQUIRE( verify_real_symbol(result, L"b") == 25 );
    REQUIRE( verify_real_symbol(result, L"c") == 27 );
}

TEST_CASE( "Interpretation succeeds - program (vars with underscore) 1", "[interpreter_program]" ) {
    const auto result = do_interpret_program(L"\
PROGRAM Semi;                           \
VAR _a: INTEGER;\
BEGIN       \
   _a := 2;  \
END.        \
");

    REQUIRE( result.output  == std::wstring(L"done") );

    REQUIRE( verify_int_symbol(result, L"_a") == 2 );
}

TEST_CASE( "Interpretation succeeds - program (vars with underscore) 2", "[interpreter_program]" ) {
     const auto result = do_interpret_program(L"\
PROGRAM Semi;                           \
VAR _a_b: INTEGER;\
BEGIN       \
   _a_b := 2;  \
END.        \
");

    REQUIRE( result.output  == std::wstring(L"done") );

    REQUIRE( verify_int_symbol(result, L"_a_b") == 2 );
}

TEST_CASE( "Interpretation succeeds - program (division using 'div' keyword)", "[interpreter_program]" ) {
    const auto result = do_interpret_program(L"\
PROGRAM Semi;                           \
VAR _a: INTEGER;\
BEGIN       \
   _a := 2 div 3;  \
END.        \
");

    REQUIRE( result.output == std::wstring(L"done") );

    REQUIRE( verify_int_symbol(result, L"_a") == 0 );
}

TEST_CASE( "Interpretation succeeds - procedures", "[interpreter_program]" ) {
    const auto result = do_interpret_program(L"\
PROGRAM Semi;                           \
VAR _a: INTEGER;\
\
PROCEDURE P1;\
BEGIN {P1}\
\
END;\
\
BEGIN       \
   _a := 4 div 2;  \
END.        \
");

    REQUIRE( result.output  == std::wstring(L"done") );

    REQUIRE( verify_int_symbol(result, L"_a") == 2 );
}