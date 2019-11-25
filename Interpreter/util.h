#pragma once
#include <string>

/**
 * For use in std::map - provides a case-insensitive way to compare strings.
 * This is not entirely unicode-correct: it assumes the strings are normalized.
 */
struct case_insensitive_string_comparer {
  bool operator() (const std::wstring& left, const std::wstring& right) const
  {
	  if (left.length() < right.length())
	  {
		  return true;
	  }

  	  if (right.length() < left.length())
	  {
		  return false;
	  }

  	  return std::equal(left.begin(), left.end(),
	                      right.begin(), right.end(),
	                      [](wchar_t a, wchar_t b) {
	                          return std::tolower(a) < std::tolower(b);
	                      });
  }
};
