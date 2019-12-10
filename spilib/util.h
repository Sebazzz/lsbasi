#pragma once

std::wstring string_to_wstring(const std::string& input);
std::string wstring_to_string(const std::wstring& input);

/**
 * For use in std::map - provides a case-insensitive way to compare strings.
 * This is not entirely unicode-correct: it assumes the strings are normalized.
 */
struct case_insensitive_string_comparer
{
    struct compare_internal
    {
      bool operator() (const wchar_t& c1, const wchar_t& c2) const {
          return std::tolower (c1) < std::tolower (c2); 
      }
    };
	
    bool operator() (const std::wstring& s1, const std::wstring& s2) const {
        return std::lexicographical_compare (s1.begin (), s1.end (), s2.begin (), s2.end (), compare_internal());
    }
};
