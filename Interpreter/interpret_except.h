#pragma once

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

