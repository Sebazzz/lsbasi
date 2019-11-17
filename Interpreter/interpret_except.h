#pragma once
#include <exception>

class interpret_except: public std::exception
{
public:
	explicit interpret_except(char const* _Message)
		: exception(_Message)
	{
	}


	explicit interpret_except(exception const& _Other)
		: exception(_Other)
	{
	}
};

