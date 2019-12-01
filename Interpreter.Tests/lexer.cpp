#include "common_test.h"
#include "../Interpreter/lexer.h"

std::wstring do_lex(lexer& lex)
{
    const auto token = lex.get_next_token();
    return token.to_string() + L" [" + std::to_wstring(token.position().line_number) + L"," + std::to_wstring(token.position().column) + L"]";
}

TEST_CASE( "Lexer tokenize test - single line", "[lexer]" ) {
    lexer lex(L"BEGIN a := 2; END.");

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [1,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [1,7]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [1,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [1,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [1,13]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [1,15]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [1,18]") );
}

TEST_CASE( "Lexer tokenize test - single line + real", "[lexer]" ) {
    lexer lex(L"BEGIN a := 2.75; END.");

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [1,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [1,7]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [1,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(real,2.75) [1,12]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [1,16]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [1,18]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [1,21]") );
}

TEST_CASE( "Lexer tokenize test - single line - division int", "[lexer]" ) {
    lexer lex(L"BEGIN a := 2 div 4; END.");

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
    lexer lex(L"BEGIN a := 2 / 4; END.");

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
    lexer lex(raw_to_wstring(R"(
BEGIN
   a := 2;
END.
)"));

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [3,4]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [3,6]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [3,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [3,10]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [4,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [4,4]") );
}

TEST_CASE( "Lexer tokenize test - procedure", "[lexer]" ) {
    lexer lex(raw_to_wstring(R"(
PROCEDURE P1;
  BEGIN
     a := 2;
  END;
)"));

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
    lexer lex(raw_to_wstring(R"(
PROCEDURE P1(a : INTEGER);
  BEGIN
     a := 2;
  END;
)"));

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

TEST_CASE( "Lexer tokenize test - multiline with comment", "[lexer]" ) {
    lexer lex(raw_to_wstring(R"(
BEGIN
   a := 2;
   { should ignore this! }
END.
)"));

    REQUIRE( do_lex(lex) == std::wstring(L"token(begin,BEGIN) [2,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(idf,a) [3,4]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(:=) [3,6]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(int,2) [3,9]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(semi) [3,10]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(end,END) [5,1]") );
    REQUIRE( do_lex(lex) == std::wstring(L"token(.) [5,4]") );
}