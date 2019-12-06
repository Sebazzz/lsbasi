#pragma once

class symbol;
using symbol_identifier = std::wstring;

using symbol_ptr = std::shared_ptr<symbol>;

template<typename T>
using symbol_type_ptr = std::shared_ptr<T>;

/* Helper function for making a pointer to AST node */
template <class T, class... TArgs>
symbol_ptr make_symbol_ptr(TArgs&&... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}

/* Helper function for making a pointer to AST node */
template <class T, class... TArgs>
symbol_type_ptr<T> make_symbol_type_ptr(TArgs&&... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}
