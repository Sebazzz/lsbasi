#include "common_test.h"
#include "util.h"
#include <map>

bool test_compare(const std::wstring& left, const std::wstring& right)
{
    const case_insensitive_string_comparer comparer;

    return comparer(left, right);
}

TEST_CASE( "Case insensitive comparison", "[util - case-insensitive-compare]" ) {
    REQUIRE( test_compare(L"BEGIN", L"BEGIN") == false );
    REQUIRE( test_compare(L"BEGIN", L"begin") == false );
    REQUIRE( test_compare(L"BEGIN", L"beGin") == false );
    REQUIRE( test_compare(L"BEGIN", L"end") == true );
    REQUIRE( test_compare(L"end", L"BEGIN") == false );
    REQUIRE( test_compare(L"END", L"end") == false );
    REQUIRE( test_compare(L"END", L"div") == false );
    REQUIRE( test_compare(L"div", L"END") == true );
    REQUIRE( test_compare(L"DIV", L"div") == false );
}

size_t add_element(std::map<std::wstring, size_t, case_insensitive_string_comparer>& map, size_t& val, const std::wstring& key)
{
    map.try_emplace(key, ++val);
    return map.size();
}

TEST_CASE( "Case insensitive comparison - std::map", "[util - case-insensitive-compare]" ) {
    std::map<std::wstring, size_t, case_insensitive_string_comparer> m;
    size_t cnt = 0;

    REQUIRE( add_element(m, cnt, L"BEGIN") == 1 );
    REQUIRE( add_element(m, cnt, L"END") == 2 );
    REQUIRE( add_element(m, cnt, L"DIV") == 3 );
    REQUIRE( add_element(m, cnt, L"VID") == 4 );
    REQUIRE( add_element(m, cnt, L"DEN") == 5 );
    REQUIRE( add_element(m, cnt, L"begin") == 5 );
    REQUIRE( add_element(m, cnt, L"end") == 5 );
    REQUIRE( add_element(m, cnt, L"div") == 5 );

}