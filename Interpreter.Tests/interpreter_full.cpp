#include "common_test.h"
#include "../Interpreter/interpreter.h"
#include "../Interpreter/symbol.h"
#include "../Interpreter/type.h"
#include "../Interpreter/procedure_symbol.h"
#include "../Interpreter/var_decl.h"
#include "../Interpreter/builtin_type_symbol.h"

struct interpret_result
{
    std::shared_ptr<scope_context> global_scope;
    interpreter_context_ptr context;
    ast_ptr ast;
    std::wstring output;
};

int verify_int_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto var_symbol = result.global_scope->symbols.get<variable_symbol>(identifier);
    const auto& contents = result.global_scope->memory->get(var_symbol);
    const auto& builtin_type = result.global_scope->symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());

    if (builtin_type->type() != ast::builtin_type::integer)
    {
        throw std::logic_error("Symbol was not an integer");
    }

    return contents.int_val;
}

double verify_real_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto var_symbol = result.global_scope->symbols.get<variable_symbol>(identifier);
    const auto& contents = result.global_scope->memory->get(var_symbol);
    const auto& builtin_type = result.global_scope->symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());

    if (builtin_type->type() != ast::builtin_type::real)
    {
        throw std::logic_error("Symbol was not an real");
    }

    return contents.real_val;
}

builtin_string verify_string_symbol(const interpret_result& result, const symbol_identifier& identifier)
{
    const auto var_symbol = result.global_scope->symbols.get<variable_symbol>(identifier);
    const auto& contents = result.global_scope->memory->get(var_symbol);
    const auto& builtin_type = result.global_scope->symbols.get<builtin_type_symbol>(var_symbol->variable().type()->identifier());

    if (builtin_type->type() != ast::builtin_type::string)
    {
        throw std::logic_error("Symbol was not an string");
    }

    return *contents.string_ptr_val;
}

interpret_result do_interpret_program(const char* input)
{
    // Replace the stdout so we can capture the interpreter output
    const auto old_wcout_buf = std::wcout.rdbuf();
    std::wstringstream wcout_stream;
    std::wcout.rdbuf(wcout_stream.rdbuf());

    // String-stream for interpreter input
    std::wstringstream input_stream;
	input_stream.str(raw_to_wstring(input));

    interpreter sut(input_stream, false);

    // Collect results
    std::wcout << sut.interpret();
    const auto inter_info = sut.get_interpretation_info();

    // Restore std::wcout stream
    std::wcout.rdbuf(old_wcout_buf);

    // ... Capture output
    wcout_stream.seekg(0);
    std::wstring interpret_output = wcout_stream.str();
    

    return {
        inter_info.global_scope,
        inter_info.interpreter_context,
        inter_info.ast,
        interpret_output
    };
}


TEST_CASE( "Implicit assignment from real to integer disallowed", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR a: INTEGER;
    b: REAL;
BEGIN       
   a := b;  
END.        
)";

    REQUIRE_THROWS_MATCHES(
		do_interpret_program(program),
		runtime_type_error, 
		Catch::Message("Runtime type error: Attempting to assign variable variable a (which is of type built-in INTEGER) from expression with invalid type: built-in REAL"));
}

TEST_CASE( "Implicit assignment from integer to real allowed", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;  
   b := a;  
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 2 );
    REQUIRE( verify_real_symbol(result, L"b") == 2 );
}

TEST_CASE( "Assignment from string to real disallowed", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR str: STRING; b: REAL;
BEGIN       
   str := 'HELLO WORLD';  
   b := str;  
END.        
)";

        REQUIRE_THROWS_MATCHES( do_interpret_program(program), 
			runtime_type_error, 
			Catch::Message("Runtime type error: Attempting to assign variable variable b (which is of type built-in REAL) from expression with invalid type: built-in STRING"));

}

TEST_CASE( "Assignment from string to integer disallowed", "[interpreter_program]" ) {
    const auto program = R"(
PROGRAM Simple;
VAR str: STRING; b: INTEGER;
BEGIN       
   str := 'HELLO WORLD';  
   b := str;  
END.        
)";

        REQUIRE_THROWS_MATCHES( do_interpret_program(program), 
			runtime_type_error, 
			Catch::Message("Runtime type error: Attempting to assign variable variable b (which is of type built-in INTEGER) from expression with invalid type: built-in STRING"));

}

TEST_CASE( "Interpretation succeeds - program with string", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Simple;
VAR a, b: STRING;
BEGIN       
   a := 'hello world';
   b := a;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_string_symbol(result, L"a") == L"hello world" );
    REQUIRE( verify_string_symbol(result, L"b") == L"hello world" );
}

TEST_CASE( "Interpretation succeeds - call to builtin procedure", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Simple;
BEGIN       
   writeln('hello world')
END.        
)");

    REQUIRE( result.output == std::wstring(L"hello world\n") );
}

TEST_CASE( "Interpretation succeeds - call to builtin procedure with uninitialized variable", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Simple;
VAR a: STRING;
BEGIN       
   writeln(a)
END.        
)");

    REQUIRE( result.output == std::wstring(L"\n") );
}

TEST_CASE( "Interpretation succeeds - program with string concatenation", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Simple;
VAR a, b, c: STRING;
BEGIN       
   a := 'hello';
   b := 'world';
   c := a + ' ' + b;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_string_symbol(result, L"a") == L"hello" );
    REQUIRE( verify_string_symbol(result, L"b") == L"world" );
    REQUIRE( verify_string_symbol(result, L"c") == L"hello world" );
}

TEST_CASE( "Interpretation succeeds - program with string concatenation and uninitialized variable", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Simple;
VAR a, b, c: STRING;
BEGIN       
   a := 'hello';
   c := a + ' ' + b;
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_string_symbol(result, L"a") == L"hello" );
    REQUIRE( verify_string_symbol(result, L"c") == L"hello " );
}

TEST_CASE( "Interpretation succeeds - program 1", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Simple;
VAR a: INTEGER; b: REAL;
BEGIN       
   a := 2;  
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"a") == 2 );
}


TEST_CASE( "Interpretation succeeds - program 2", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
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

	REQUIRE(result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"x") == 11 );
    REQUIRE( verify_int_symbol(result, L"number") == 2 );

    REQUIRE( verify_real_symbol(result, L"a") == 2 );
    REQUIRE( verify_real_symbol(result, L"b") == 25 );
    REQUIRE( verify_real_symbol(result, L"c") == 27 );
}

TEST_CASE( "Interpretation succeeds - program (case insensitive)", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
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

	REQUIRE(result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"x") == 11 );
    REQUIRE( verify_int_symbol(result, L"number") == 2 );

    REQUIRE( verify_real_symbol(result, L"a") == 2 );
    REQUIRE( verify_real_symbol(result, L"b") == 25 );
    REQUIRE( verify_real_symbol(result, L"c") == 27 );
}

TEST_CASE( "Interpretation succeeds - program (vars with underscore) 1", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;
BEGIN       
   _a := 2;  
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a") == 2 );
}

TEST_CASE( "Interpretation succeeds - program (vars with underscore) 2", "[interpreter_program]" ) {
     const auto result = do_interpret_program(R"(
PROGRAM Semi;                           
VAR _a_b: INTEGER;
BEGIN       
   _a_b := 2;  
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a_b") == 2 );
}

TEST_CASE( "Interpretation succeeds - program (division using 'div' keyword)", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;
BEGIN       
   _a := 2 div 3;  
END.        
)");

    REQUIRE( result.output == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a") == 0 );
}

TEST_CASE( "Interpretation succeeds - procedures", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
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

TEST_CASE( "Interpretation succeeds - functions", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Semi;                           
VAR _a: INTEGER;

FUNCTION F1: INTEGER;
BEGIN {F1}
    F1 := 4 div 2
END;

BEGIN       
   _a := F1();  
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( verify_int_symbol(result, L"_a") == 2 );
}

TEST_CASE( "Interpretation succeeds - procedures with parameters", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
PROGRAM Semi;                           
PROCEDURE P1(a : INTEGER);
BEGIN {P1}
    a:=2
END;

BEGIN       
END.        
)");

    REQUIRE( result.output  == std::wstring(L"") );

    REQUIRE( (!!result.global_scope->symbols.get<procedure_symbol>(L"P1")) == true );
}

TEST_CASE( "Interpretation succeeds - procedures with parameters called", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
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

    REQUIRE( (!!result.global_scope->symbols.get<procedure_symbol>(L"P1")) == true );
    REQUIRE( verify_int_symbol(result, L"a_global") == 5 );
}

TEST_CASE( "Interpretation succeeds - procedures with parameters called with expression", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
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

    REQUIRE( (!!result.global_scope->symbols.get<procedure_symbol>(L"P1")) == true );
    REQUIRE( verify_int_symbol(result, L"a_global") == 20 );
}

TEST_CASE( "Interpretation succeeds - procedure variable hides global variable and sets global variable", "[interpreter_program]" ) {
    const auto result = do_interpret_program(R"(
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
		do_interpret_program(program), 
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
		do_interpret_program(program), 
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
		do_interpret_program(program), 
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
		do_interpret_program(program), 
		interpret_except, 
		Catch::Message("In a call to procedure P1 no argument has been provided for this parameter: a"));
}