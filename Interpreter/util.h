#pragma once
#include <string>
#include <algorithm>

/**
 * For use in std::map - provides a case-insensitive way to compare strings.
 * This is not entirely unicode-correct: it assumes the strings are normalized.
 */
struct case_insensitive_string_comparer
{
	bool operator()(const std::wstring& s1, const std::wstring& s2) const
	{
		// This implementation works, but is suboptimal because it allocates
		// a lot for a simple comparison. TODO: find a better way
		
		std::wstring str1(s1.length(), ' ');
		std::wstring str2(s2.length(), ' ');

		const auto myLambda = [](wchar_t c) -> wchar_t { return towlower(c); };
		std::transform(s1.begin(), s1.end(), str1.begin(), myLambda);
		std::transform(s2.begin(), s2.end(), str2.begin(), myLambda);

		return str1 < str2;
	}
};
