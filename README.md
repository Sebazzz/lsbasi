# Simple Pascal Interpreter

This is my attempt at following the [Let's Build A Simple Interpreter](https://ruslanspivak.com/lsbasi-part1/) series. Since I like C++ but probably never will write C++ professionally (full-time C# web developer) I decided that C++ should be a good exercise for me.

Implemented language features:

- Built-in types
	- BOOLEAN
	- INTEGER
    - REAL
	- STRING (*TODO: Currently leaks memory because the string pool is not cleaned up when strings are no longer used*)
- Variable assignment
    - Type conversion
- Operators
    - Standard math operators
    - Comparison operators (*Just eq for now*)
	- String concatenation
	- Unary operators
- Control flow
    - If / Else
        - Single line
        - Multi-line
- Semantic analysis
- Comments
- Procedures
    - Parameter support
    - Built-in procedures
        - `writeln`
- Functions
    - Parameter support
    - Built-in functions
        - `random`
	
Implemented interpreter features:

- REPL mode
- File mode
- Pinpointing the cause of an issue, for instance: 

```
In a call to procedure writeln too many arguments have been provided

           writeln('a = ', a);
        -------------------^
```

Technical:

- Written in C++17
- Minimum dependency on Win32 API if any
- Warnings as errors, SDL checks on
- About 60 unit tests written in C++ / Catch
	
**Sample program:**
``` pascal
PROGRAM Part10;
VAR
   number     : INTEGER;
   a, b, c, x : INTEGER;
   y          : REAL;

BEGIN {Part10}
   BEGIN
      number := 2;
      a := number;
      b := 10 * a + 10 * number DIV 4;
      c := a - - b
   END;
   x := 11;
   y := 20 / 7 + 3.14;
   writeln('hello world!');
END.  {Part10}
```
