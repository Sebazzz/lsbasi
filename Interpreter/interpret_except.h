#pragma once

std::wstring string_to_wstring(const std::string& input);
std::string wstring_to_string(const std::wstring& input);

class interpret_except: public std::exception
{
private:
	std::string m_message;

public:
	virtual ~interpret_except() = default;
	
	explicit interpret_except(const std::string& message);
	explicit interpret_except(const std::wstring& message);

	explicit interpret_except(exception const& _Other);

	[[nodiscard]] char const* what() const override;
};

