#pragma once

enum class token_type
{
	integer,
	plus,
	minus,
	multiply,
	divide,
	group_start,
	group_end,
	eof
};