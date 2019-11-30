 #include "catch.hpp"
#include "../Interpreter/parser.h"
#include "../Interpreter/symbol.h"
#include "../Interpreter/symbol_table_builder.h"

struct parse_result
{
    std::shared_ptr<symbol_table> symbol_table;
    ast::ast_ptr ast;
};

parse_result do_parse_program(std::wstring input)
{
    parser sut(std::move(input));

    const auto result = sut.parse();

    symbol_table_builder symbol_visitor;
    symbol_visitor.visit(*result);

    return {
        symbol_visitor.symbol_table(),
        result
    };
}


TEST_CASE( "Symbol lookup succeeds - program 1", "[symbol_table_builder]" ) {
    const auto result = do_parse_program(L"\
PROGRAM Simple;\
VAR a: INTEGER; b: REAL;\
BEGIN       \
   a := 2;  \
END.        \
");

    REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"b")) == typeid(variable_symbol) );
}


TEST_CASE( "Symbol lookup succeeds - program 2", "[symbol_table_builder]" ) {
    const auto result = do_parse_program(L"\
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

    REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"b")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"c")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"x")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"number")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - program (case insensitive)", "[symbol_table_builder]" ) {
    const auto result = do_parse_program(L"\
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

	REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"b")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"c")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"x")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"number")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - program (vars with underscore) 1", "[symbol_table_builder]" ) {
    const auto result = do_parse_program(L"\
PROGRAM Semi;                           \
VAR _a: INTEGER;\
BEGIN       \
   _a := 2;  \
END.        \
");

	REQUIRE( typeid(*result.symbol_table->get(L"_a")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - program (vars with underscore) 2", "[symbol_table_builder]" ) {
     const auto result = do_parse_program(L"\
PROGRAM Semi;                           \
VAR _a_b: INTEGER;\
BEGIN       \
   _a_b := 2;  \
END.        \
");

	REQUIRE( typeid(*result.symbol_table->get(L"_a_b")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - procedures", "[symbol_table_builder]" ) {
    const auto result = do_parse_program(L"\
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

	REQUIRE( typeid(*result.symbol_table->get(L"_a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(procedure_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - procedure with parameters", "[symbol_table_builder]" ) {
    const auto result = do_parse_program(L"\
PROGRAM Semi;                           \
VAR _a: INTEGER;\
PROCEDURE P1(a : INTEGER);\
BEGIN {P1}\
    _a:=a \
END;\
\
BEGIN       \
END.        \
");

	REQUIRE( typeid(*result.symbol_table->get(L"_a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(procedure_symbol) );

    auto proc_symbol = result.symbol_table->get<procedure_symbol>(L"P1");
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"a")) == typeid(variable_symbol) );
}


TEST_CASE( "Symbol lookup succeeds - procedure with multiple parameters", "[symbol_table_builder]" ) {
    const auto result = do_parse_program(L"\
PROGRAM Semi;                           \
VAR _a: INTEGER;\
PROCEDURE P1(a, b : INTEGER; c: REAL);\
BEGIN {P1}\
    _a:=a + b + c \
END;\
\
BEGIN       \
END.        \
");

	REQUIRE( typeid(*result.symbol_table->get(L"_a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(procedure_symbol) );

    auto proc_symbol = result.symbol_table->get<procedure_symbol>(L"P1");
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"b")) == typeid(variable_symbol) );
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"c")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - nested procedures", "[symbol_table_builder]" ) {
    const auto result = do_parse_program(L"\
PROGRAM Part12;\
VAR\
   a : INTEGER;\
\
PROCEDURE P1;\
VAR\
   a : REAL;\
   k : INTEGER;\
\
   PROCEDURE P2;\
   VAR\
      a, z : INTEGER;\
   BEGIN {P2}\
      z := 777;\
   END;  {P2}\
\
BEGIN {P1}\
\
END;  {P1}\
\
BEGIN {Part12}\
   a := 10;\
END.  {Part12}\
");

	REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(procedure_symbol) );

    auto proc_symbol = result.symbol_table->get<procedure_symbol>(L"P1");
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"P2")) == typeid(procedure_symbol) );
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"k")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol duplicate declaration fails - program 1", "[symbol_table_builder]" ) {
    const auto program = L"\
PROGRAM Simple;\
VAR b: INTEGER;\
    b: REAL;\
BEGIN       \
   b := 2;  \
END.        \
";

    REQUIRE_THROWS_MATCHES( do_parse_program(program), interpret_except, Catch::Message("Attempt to declare duplicate symbol 'variable b' which already exists in this scope: Simple"));
}
    
TEST_CASE( "Symbol lookup fails - program 1", "[symbol_table_builder]" ) {
    const auto program = L"\
PROGRAM Simple;\
VAR b: REAL;\
BEGIN       \
   a := 2;  \
END.        \
";

    REQUIRE_THROWS_MATCHES( do_parse_program(program), interpret_except, Catch::Message("Attempt to reference symbol with name 'a' which does not exist in this scope: Simple"));
}

TEST_CASE( "Symbol lookup fails - program 2", "[symbol_table_builder]" ) {
    const auto program = L"\
PROGRAM Simple;\
VAR a: NOTEXISTS;\
BEGIN       \
   a := 2;  \
END.        \
";

    REQUIRE_THROWS_MATCHES( do_parse_program(program), interpret_except, Catch::Message("Attempt to reference symbol with name 'NOTEXISTS' which does not exist in this scope: Simple"));
}