#pragma once

class symbol;
using symbol_identifier = std::wstring;

using symbol_ptr = std::shared_ptr<symbol>;

template<typename T>
using symbol_type_ptr = std::shared_ptr<T>;

/* Helper function for making a shared pointer to symbol */
template <class T, class... TArgs>
symbol_type_ptr<T> make_symbol_ptr(TArgs&&... args)
{
	static_assert(std::is_convertible<T*, symbol*>::value, "This template is only suitable for symbols");
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}
