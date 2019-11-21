#pragma once

enum class token_type
{
	// data types
	integer,

	// operators
	plus,
	minus,
	multiply,
	divide,

	// constructs
	group_start,
	group_end,

	// program definition
	begin,
	end,
	dot,
	assign,
	semicolon,
	identifier,
	
	eof
};