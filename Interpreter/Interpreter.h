#pragma once
#include <string>
#include <utility>
#include <memory>
#include "Token.h"
#include <vector>
#include <optional>

class Interpreter
{
private:
	std::wstring input;
	std::vector<Token> tokens;

	std::optional<Token> currentToken;
	size_t pos;

	void tokenizeIfNecessary();
	void tokenizeCore();
	Token getNextToken();

public:
	explicit Interpreter(std::wstring input)
		: input(std::move(input))
	{
		this->tokens.reserve(3);
		this->pos = 0;
	}

	std::wstring tokenize();
	int parseIntegerToken(const std::vector<Token>::value_type& token) const;
	std::wstring interpret();
};

