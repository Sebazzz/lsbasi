// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "interpreter.h"
#include "interpret_except.h"
#include "console_color.h"
#include "rpn_visitor.h"

std::wstring make_rpn(std::wstring& input)
{
	parser parser(std::move(input));
    const auto result = parser.parse();

    rpn_visitor visitor;
    visitor.visit(*result);

    return visitor.get_string();
}

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
			interpreter interpreter(input);

			std::wstring tokenStr = interpreter.tokenize();
			std::wcout << L"Tokens: " << tokenStr << std::endl;

			std::wstring astStr = interpreter.stringify_ast();
			std::wcout << L"AST:    " << astStr << std::endl;

			std::wstring rpn = make_rpn(input);
			std::wcout << L"RPN:    " << rpn << std::endl;

			std::wstring result = interpreter.interpret();
			std::wcout << L"Result: " << result << std::endl;
		} catch (interpret_except& e)
		{
			std::wcout << console_color::reversed << console_color::bright_red << console_color::bold << L"Error interpreting --> " <<  console_color::faint;
			std::wcout << console_color::red << e.what() << console_color::reset << std::endl;
		}
		
		std::wcout << std::endl;
	}
}
