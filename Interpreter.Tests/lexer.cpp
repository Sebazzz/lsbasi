#include "common_test.h"
#include "../Interpreter/lexer.h"
#include "../Interpreter/parse_except.h"

std::unique_ptr<std::wstringstream> make_lexer_input_stream(const char* input)
{
    std::unique_ptr<std::wstringstream> input_stream(new std::wstringstream());
	input_stream->str(raw_to_wstring(input));

    return input_stream;
}

std::wstring do_lex(lexer& lex)
{
    const auto token = lex.get_next_token();
    return token.to_string() + L" [" + std::to_wstring(token.position().line_number) + L"," + std::to_wstring(token.position().column) + L"]";
}

TEST_CASE( "Lexer tokenize test - single line", "[lexer]" ) {
    const auto input = make_lexer_input_stream("BEGIN a := 2; END.");
    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [1,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [1,7]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [1,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [1,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [1,13]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [1,15]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [1,18]") );
}

TEST_CASE( "Lexer tokenize test - single line + real", "[lexer]" ) {
    const auto input = make_lexer_input_stream("BEGIN a := 2.75; END.");
    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [1,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [1,7]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [1,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(real,2.75) [1,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [1,16]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [1,18]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [1,21]") );
}

TEST_CASE( "Lexer tokenize test - single line - division int", "[lexer]" ) {
    const auto input = make_lexer_input_stream("BEGIN a := 2 div 4; END.");
    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [1,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [1,7]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [1,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [1,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idiv,DIV) [1,14]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,4) [1,18]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [1,19]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [1,21]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [1,24]") );
}

TEST_CASE( "Lexer tokenize test - single line - division real", "[lexer]" ) {
    const auto input = make_lexer_input_stream("BEGIN a := 2 / 4; END.");
    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [1,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [1,7]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [1,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [1,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(rdiv) [1,14]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,4) [1,16]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [1,17]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [1,19]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [1,22]") );
}

TEST_CASE( "Lexer tokenize test - multiline", "[lexer]" ) {
    const auto input = make_lexer_input_stream(R"(
BEGIN
   a := 2;
END.
)");

    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [3,4]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [3,6]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [3,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [3,10]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [4,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [4,4]") );
}

TEST_CASE( "Lexer tokenize test - string support", "[lexer]" ) {
    const auto input = make_lexer_input_stream(R"(
BEGIN
   a := 'my string long long';
END.
)");

    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [3,4]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [3,6]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(str,my string long long) [3,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [3,30]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [4,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [4,4]") );
}

TEST_CASE( "Lexer tokenize test - string support (unterminated results in error)", "[lexer]" ) {
    const auto input = make_lexer_input_stream(R"(
BEGIN
   a := 'my string long long;
END.
)");

    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [3,4]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [3,6]") );

    REQUIRE_THROWS_MATCHES( 
		do_lex(lex), 
		parse_except, 
		Catch::Message("Syntax error: Unterminated string literal: my string long long;"));
} 

TEST_CASE( "Lexer tokenize test - procedure", "[lexer]" ) {
    const auto input = make_lexer_input_stream(R"(
PROCEDURE P1;
  BEGIN
     a := 2;
  END;
)");

    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(procedure,PROCEDURE) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,P1) [2,11]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [2,13]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [3,3]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [4,6]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [4,8]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [4,11]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [4,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [5,3]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [5,6]") );
}

TEST_CASE( "Lexer tokenize test - procedure with params", "[lexer]" ) {
    const auto input = make_lexer_input_stream(R"(
PROCEDURE P1(a : INTEGER);
  BEGIN
     a := 2;
  END;
)");

    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(procedure,PROCEDURE) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,P1) [2,11]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(group_start) [2,13]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [2,14]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(colon) [2,16]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,INTEGER) [2,18]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(group_end) [2,25]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [2,26]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [3,3]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [4,6]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [4,8]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [4,11]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [4,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [5,3]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [5,6]") );
}

TEST_CASE( "Lexer tokenize test - boolean", "[lexer]" ) {
    const auto input = make_lexer_input_stream(R"(
VAR a : BOOLEAN;
BEGIN
 a := true;
END;
)");

    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(var_decl,VAR) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [2,5]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(colon) [2,7]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,BOOLEAN) [2,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [2,16]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [3,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [4,2]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [4,4]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(bool,true) [4,7]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [4,11]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [5,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [5,4]") );
}

TEST_CASE( "Lexer tokenize test - function with params", "[lexer]" ) {
    const auto input = make_lexer_input_stream(R"(
FUNCTION F1(a : INTEGER): INTEGER;
  BEGIN
     F1 := a;
  END;
)");

    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(function,FUNCTION) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,F1) [2,10]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(group_start) [2,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [2,13]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(colon) [2,15]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,INTEGER) [2,17]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(group_end) [2,24]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(colon) [2,25]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,INTEGER) [2,27]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [2,34]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [3,3]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,F1) [4,6]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [4,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [4,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [4,13]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [5,3]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [5,6]") );
}

TEST_CASE( "Lexer tokenize test - multiline with comment", "[lexer]" ) {
    const auto input = make_lexer_input_stream(R"(
BEGIN
   a := 2;
   { should ignore this! }
END.
)");

    lexer lex(*input);

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [3,4]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [3,6]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [3,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [3,10]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [5,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [5,4]") );
}