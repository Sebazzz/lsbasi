// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "interpreter.h"
#include "console_color.h"
#include "rpn_visitor.h"
#include "invoke_repl_visitor.h"
#include "lisp_notation_visitor.h"

std::wstring make_rpn(const std::wstring& input)
{
    return invoke_repl_visitor<rpn_visitor>(input);
}

std::wstring make_lisp_notation(const std::wstring& input)
{
    return invoke_repl_visitor<lisp_notation_visitor>(input);
}

void repl_mode()
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
			std::wstringstream input_stream;
			input_stream.str(input);
			
			{
				interpreter interpreter(input_stream, true);

				std::wstring tokenStr = interpreter.tokenize();
				std::wcout << L"Tokens: " << tokenStr << std::endl;

				input_stream.seekg(0);
			}

			{
				interpreter interpreter(input_stream, true);

				std::wstring astStr = interpreter.stringify_ast();
				std::wcout << L"AST:    " << astStr << std::endl;

				std::wstring rpn = make_rpn(input);
				std::wcout << L"RPN:    " << rpn << std::endl;

				std::wstring lisp = make_lisp_notation(input);
				std::wcout << L"LISP:   " << lisp << std::endl;

				std::wstring result = interpreter.interpret();
				std::wcout << L"Result: " << result << std::endl;
				
				input_stream.seekg(0);
			}
		} catch (interpret_except& e)
		{
			std::wcout << console_color::reversed << console_color::bright_red << console_color::bold << L"Error interpreting --> " <<  console_color::faint;
			std::wcout << console_color::red << e.what() << console_color::reset << std::endl;

			// Give some indication where the error has occurred, if we have the information
			if (e.has_line_info())
			{
				const int col = e.line_info().column;
				
				std::wcout << std::endl << "\t" << input << std::endl;
				std::wcout << "\t" << std::wstring(col - 1, L'-') << console_color::bright_red << L'^' << console_color::reset << std::endl;
			}
		}
		
		std::wcout << std::endl;
	}
}

int interpret_file(const std::wstring& file_path)
{
	std::wcout << "Interpreting: " << file_path << std::endl;
	
	std::wifstream file(file_path);

	if (file.bad())
	{
		std::wcout << "Unable to open file." << std::endl;
		return -1;
	}
	
	interpreter interpreter(file);
	try
	{
		std::wcout << interpreter.interpret() << std::endl;
	} catch (interpret_except& e)
	{
		std::wcout << "Error interpreting [" << file_path << "]" << std::endl;
		std::wcout << e.what() << std::endl;

		return -1;
	}

	std::wcout << "Done." << std::endl;

	return 0;
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc <= 1)
	{
		std::wcout << "No arguments were given. Welcome to REPL mode" << std::endl << std::endl;

		repl_mode();

		return 0;
	}

	const std::wstring first_arg = argv[1];
	if (first_arg == L"--help")
	{
		std::wcout << "SD Pascal Interpreter" << std::endl;
		std::wcout << "Usage: Interpreter.exe [file]" << std::endl;
		std::wcout << std::endl;
		return 0;
	}

	return interpret_file(first_arg);
}
