// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Interpreter.h"
#include "interpret_except.h"
#include "console_color.h"

int main()
{
	std::wcout << L"Enter your expressions" << std::endl;

	while (true)
	{
		std::wstring input;
		input.reserve(256);
		
		std::wcout << L"> ";
		std::getline(std::wcin, input);
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
			std::wcout << console_color::reversed << console_color::bright_red << console_color::bold << L"Error interpreting --> " <<  console_color::faint;
			std::wcout << console_color::red << e.what() << console_color::reset << std::endl;
		}
		
		std::wcout << std::endl;
	}
}
