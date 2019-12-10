#include "common_test.h"
#include "parser.h"
#include "symbol.h"
#include "routine_symbol.h"

TEST_CASE( "Symbol lookup succeeds - program 1", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;  
END.        
)");

    REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"b")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - program with boolean", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Simple;
VAR a: BOOLEAN;
BEGIN       
   a := true;  
END.        
)");

    REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );

    auto var_symbol = result.symbol_table->get<variable_symbol>(L"a");
    REQUIRE( var_symbol->variable().type()->type_symbol()->to_string() == std::wstring(L"built-in BOOLEAN") );
}

TEST_CASE( "Symbol lookup succeeds - program 2", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Semi;                           
VAR a, b, c: REAL;
    x, number: INTEGER;
BEGIN                                   
    BEGIN                               
        number := 2;                    
        a := number;                    
        b := 10 * a + 10 * number / 4;  
        c := a - - b                    
    END;                                
    x := 11;                            
END.                                    
)");

    REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"b")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"c")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"x")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"number")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - program (case insensitive)", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Semi;                           
VAR a, b, c: REAL;
    x, number: INTEGER;
begIN                                   
    BegiN                               
        NUMBer := 2;                    
        a := numBER;                    
        b := 10 * A + 10 * number / 4;  
        c := A - - B                    
    end;                                
    X := 11;                            
ENd.                                    
)");

	REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"b")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"c")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"x")) == typeid(variable_symbol) );
    REQUIRE( typeid(*result.symbol_table->get(L"number")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - program (vars with underscore) 1", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;
BEGIN       
   _a := 2;  
END.        
)");

	REQUIRE( typeid(*result.symbol_table->get(L"_a")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - program (vars with underscore) 2", "[symbol_table_builder]" ) {
     const auto result = test_symbol_table_builder(R"(
PROGRAM Semi;                           
VAR _a_b: INTEGER;
BEGIN       
   _a_b := 2;  
END.        
)");

	REQUIRE( typeid(*result.symbol_table->get(L"_a_b")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - procedures", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;

PROCEDURE P1;
BEGIN {P1}

END;

BEGIN       
   _a := 4 div 2;  
END.        
)");

	REQUIRE( typeid(*result.symbol_table->get(L"_a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(user_defined_routine_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - procedures declared not in order", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Semi;                           

PROCEDURE P1;
BEGIN
    P2()
END;

PROCEDURE P2;
BEGIN

END;

BEGIN       
END.        
)");

	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(user_defined_routine_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P2")) == typeid(user_defined_routine_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - procedure with parameters", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Semi;
VAR _a: INTEGER;
PROCEDURE P1(a : INTEGER);
BEGIN {P1}
    _a:=a 
END;

BEGIN
END.
)");

	REQUIRE( typeid(*result.symbol_table->get(L"_a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(user_defined_routine_symbol) );

    auto proc_symbol = result.symbol_table->get<user_defined_routine_symbol>(L"P1");
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"a")) == typeid(variable_symbol) );
}


TEST_CASE( "Symbol lookup succeeds - procedure with multiple parameters", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Semi;
VAR _a: INTEGER;
PROCEDURE P1(a, b : INTEGER; c: REAL);
BEGIN {P1}
    _a:=a + b + c
END;

BEGIN
END.
)");

	REQUIRE( typeid(*result.symbol_table->get(L"_a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(user_defined_routine_symbol) );

    auto proc_symbol = result.symbol_table->get<user_defined_routine_symbol>(L"P1");
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"b")) == typeid(variable_symbol) );
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"c")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - nested procedures", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Part12;
VAR
   a : INTEGER;

PROCEDURE P1;
VAR
   a : REAL;
   k : INTEGER;

   PROCEDURE P2;
   VAR
      a, z : INTEGER;
   BEGIN {P2}
      z := 777;
   END;  {P2}

BEGIN {P1}

END;  {P1}

BEGIN {Part12}
   a := 10;
END.  {Part12}
)");

	REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(user_defined_routine_symbol) );

    auto proc_symbol = result.symbol_table->get<user_defined_routine_symbol>(L"P1");
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"P2")) == typeid(user_defined_routine_symbol) );
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"k")) == typeid(variable_symbol) );
}

TEST_CASE( "Symbol lookup succeeds - nested procedure variable hides global variable", "[symbol_table_builder]" ) {
    const auto result = test_symbol_table_builder(R"(
PROGRAM Part12;
VAR
   a : REAL; b : INTEGER;

PROCEDURE P1(a : REAL);
BEGIN {P1}
    b := a;
END;  {P1}

BEGIN {Part12}
   a := 10;
   b := 1337;
   P1(b);
END.  {Part12}
)");

	REQUIRE( typeid(*result.symbol_table->get(L"a")) == typeid(variable_symbol) );
	REQUIRE( typeid(*result.symbol_table->get(L"P1")) == typeid(user_defined_routine_symbol) );

    auto proc_symbol = result.symbol_table->get<user_defined_routine_symbol>(L"P1");
	REQUIRE( typeid(*proc_symbol->procedure().symbol_table().get(L"a")) == typeid(variable_symbol) );
	REQUIRE( proc_symbol->procedure().symbol_table().get(L"b").get() == result.symbol_table->get(L"b").get());
	REQUIRE( proc_symbol->procedure().symbol_table().get(L"a").get() != result.symbol_table->get(L"a").get());
}

TEST_CASE( "Symbol duplicate declaration fails - program 1", "[symbol_table_builder]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR b: INTEGER;
    b: REAL;
BEGIN       
   b := 2;  
END.        
)";

    REQUIRE_THROWS_MATCHES( test_symbol_table_builder(program), interpret_except, Catch::Message("Attempt to declare duplicate symbol 'variable b' which already exists in this scope: Simple"));
}
    
TEST_CASE( "Symbol lookup fails - program 1", "[symbol_table_builder]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR b: REAL;
BEGIN       
   a := 2;  
END.        
)";

    REQUIRE_THROWS_MATCHES( 
		test_symbol_table_builder(program), 
		interpret_except, 
		Catch::Message("Attempt to reference symbol with name 'a' which does not exist in this scope: Simple"));
}

TEST_CASE( "Symbol lookup fails - program 2", "[symbol_table_builder]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR a: NOTEXISTS;
BEGIN       
   a := 2;  
END.        
)";

    REQUIRE_THROWS_MATCHES( 
		test_symbol_table_builder(program), 
		interpret_except, 
		Catch::Message("Unable to resolve symbol of type 'class type_symbol': Attempt to reference symbol with name 'NOTEXISTS' which does not exist in this scope: Simple"));
}

TEST_CASE( "Symbol lookup fails - program undefined procedure", "[symbol_table_builder]" ) {
    const auto program = R"(
PROGRAM Simple;
BEGIN       
   calculate();
END.        
)";

    REQUIRE_THROWS_MATCHES( 
		test_symbol_table_builder(program), 
		interpret_except, 
		Catch::Message("Unable to resolve symbol of type 'class routine_symbol': Attempt to reference symbol with name 'calculate' which does not exist in this scope: Simple"));
}

TEST_CASE( "Symbol lookup fails - program undefined procedure args", "[symbol_table_builder]" ) {
    const auto program = R"(
PROGRAM Simple;
PROCEDURE calculate(a, b : INTEGER; c: REAL);
BEGIN {P1}
    a := b + c;
END;
BEGIN       
   calculate(x,y,z);
END.        
)";

    REQUIRE_THROWS_MATCHES( 
		test_symbol_table_builder(program), 
		interpret_except, 
		Catch::Message("In call to procedure 'calculate' unexpected error found: Attempt to reference symbol with name 'x' which does not exist in this scope: Simple"));
}