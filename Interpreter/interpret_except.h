#pragma once

class interpret_except: public std::exception
{
private:
	std::string m_message;

public:
	explicit interpret_except(const std::string& message);

	explicit interpret_except(const std::string& message, const std::string& actual);

	explicit interpret_except(const std::string& message, const std::wstring& actual);
	
	explicit interpret_except(const std::wstring& message);

	explicit interpret_except(const std::string& message, const wchar_t* actual);
	
	explicit interpret_except(const std::wstring& message, const char* actual);

	explicit interpret_except(exception const& _Other);

	[[nodiscard]] char const* what() const override;
};

