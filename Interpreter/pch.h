#pragma once

// Interpreter PCH file for common includes and reduced compilation time

// STL
#include <algorithm>
#include <exception>
#include <map>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Private
#include "ast_common.h"
#include "ast_node.h"
#include "builtin_type_defs.h"
#include "interpret_except.h"
#include "parse_except.h"
#include "semantic_except.h"
#include "internal_interpret_except.h"
#include "token.h"
#include "line_info.h"
#include "util.h"