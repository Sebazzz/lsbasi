#pragma once

enum class token_type
{
	// data values
	boolean_const,
	integer_const,
	real_const,
	string_const,

	// data types
	type,
	var_decl,

	// operators (ref: https://wiki.freepascal.org/Operator)
	multiply,
	divide_integer,
	divide_real,
	
	plus,
	minus,

	compare_equal,

	// constructs
	group_start,
	group_end,

	// program definition
	program,
	procedure,
	function,
	
	begin,
	end,

	if_ctrl,
	then_ctrl,
	else_ctrl,
	
	assign,
	comma,
	colon,
	semicolon,
	identifier,
	dot,
	
	eof
};