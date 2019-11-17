// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Interpreter.h"
#include "interpret_except.h"
#include "ConsoleColor.h"

int main()
{
	std::wcout << L"Enter your expressions" << std::endl;

	while (true)
	{
		std::wstring input;
		input.reserve(256);
		
		std::wcout << L"> ";
		std::wcin >> input;
		std::wcout << std::endl;

		try
		{
			Interpreter interpreter(input);

			std::wstring tokenStr = interpreter.tokenize();
			std::wcout << tokenStr << std::endl;

			std::wstring result = interpreter.interpret();
			std::wcout << result << std::endl;
		} catch (interpret_except& e)
		{
			std::wcout << consolecolor::reversed << consolecolor::red << consolecolor::bold << L"Error interpreting --> " <<  consolecolor::reset;
			std::wcout << consolecolor::reversed << consolecolor::red << e.what() << consolecolor::reset << std::endl;
		}
		
		std::wcout << std::endl;
	}
}
