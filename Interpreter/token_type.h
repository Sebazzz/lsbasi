#pragma once

enum class token_type
{
	// data values
	integer_const,
	real_const,
	string_const,

	// data types
	type,
	var_decl,

	// operators
	plus,
	minus,
	multiply,
	divide_integer,
	divide_real,

	// constructs
	group_start,
	group_end,

	// program definition
	program,
	procedure,
	function,
	begin,
	end,
	dot,
	assign,
	comma,
	colon,
	semicolon,
	identifier,
	
	eof
};