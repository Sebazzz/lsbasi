#pragma once
#include "lexer.h"

/**
 * This is a helper class that makes iterating over the lexer tokens much easier compared to string or vector iterators.
 * It is able to throw specific error messages related to parsing and interpretation.
 *
 * As an added bonus it actually allows abstracting away where the tokens come from: whether
 * they are streaming from the network, file, or in-memory string.
 */
class lexer_iterator
{
private:
	lexer& m_lexer;
	token m_current_token;

	void expect_internal(token_type type) const;

public:
	explicit lexer_iterator(lexer& lexer);

	/**
	 * Assert the current token is the given token type and skips it. This function throws
	 * if the token is not of the expected type, or if we have reached the end of the file.
	 */
	void skip_required(token_type type);

	/**
	 * Advances to the next token. This has to be called at least once before the iterator is an usable state.
	 */
	void advance();

	/**
	 * Gets the current token. This function never throws, but may return an "eof" token instead
	 */
	[[nodiscard]] const token& current_token() const noexcept;

	/**
	 * Gets the current token. This function throws if not the expected type. The resulting token is returned.
	 */
	[[nodiscard]] const token& expect(token_type type) const;

	/**
	 * Gets the current token. This function throws if at the end of the file.
	 */
	const token* operator->() const;

	/**
	 * Gets whether we have reached the end
	 */
	bool is_at_end() const;

	/*
	 * Ensures the final token has not been reached. This function always throws, but centralizes error throwing.
	 */
	void ensure_token(const std::string& except_message) const;
};

