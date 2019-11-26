#include "catch.hpp"
#include "../Interpreter/lexer.h"

std::wstring do_lex(lexer& lex)
{
    const auto token = lex.get_next_token();
    return token.to_string();
}

TEST_CASE( "Lexer tokenize test - single line", "[lexer]" ) {
    lexer lex(L"BEGIN a := 2; END.");

    REQUIRE( do_lex(lex) == std::wstring(L"TOK(begin,BEGIN)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(idf,a)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(:=)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(int,2)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(semi)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(end,END)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(.)") );
}

TEST_CASE( "Lexer tokenize test - single line + real", "[lexer]" ) {
    lexer lex(L"BEGIN a := 2.75; END.");

    REQUIRE( do_lex(lex) == std::wstring(L"TOK(begin,BEGIN)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(idf,a)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(:=)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(real,2.75)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(semi)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(end,END)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(.)") );
}

TEST_CASE( "Lexer tokenize test - single line - division int", "[lexer]" ) {
    lexer lex(L"BEGIN a := 2 div 4; END.");

    REQUIRE( do_lex(lex) == std::wstring(L"TOK(begin,BEGIN)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(idf,a)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(:=)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(int,2)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(idiv, DIV)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(int,4)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(semi)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(end,END)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(.)") );
}

TEST_CASE( "Lexer tokenize test - single line - division real", "[lexer]" ) {
    lexer lex(L"BEGIN a := 2 / 4; END.");

    REQUIRE( do_lex(lex) == std::wstring(L"TOK(begin,BEGIN)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(idf,a)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(:=)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(int,2)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(rdiv)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(int,4)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(semi)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(end,END)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(.)") );
}

TEST_CASE( "Lexer tokenize test - multiline", "[lexer]" ) {
    lexer lex(L"\
BEGIN\
   a := 2;\
END.");

    REQUIRE( do_lex(lex) == std::wstring(L"TOK(begin,BEGIN)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(idf,a)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(:=)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(int,2)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(semi)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(end,END)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(.)") );
}

TEST_CASE( "Lexer tokenize test - multiline with comment", "[lexer]" ) {
    lexer lex(L"\
BEGIN\
   a := 2;\
   { should ignore this! }\
END.");

    REQUIRE( do_lex(lex) == std::wstring(L"TOK(begin,BEGIN)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(idf,a)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(:=)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(int,2)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(semi)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(end,END)") );
    REQUIRE( do_lex(lex) == std::wstring(L"TOK(.)") );
}