#pragma once
#include <string>

enum class Type {
	plain,
	xml,
	json,
	html,
	unknown
};
class TextOut
{	
private:
	std::string m_filename;
	std::string writePlainTag(const char* tag, const std::string& value);
	std::string writeXMLTag(const char* tag, const std::string& value);
	std::string writeJsonTag(const char* tag, const std::string& value);
	std::string writeHtmlTag(const char* tag, const std::string& value);
public:
	TextOut() = default;
	virtual ~TextOut() = default;
	void setFilename(const char *fn) {
		m_filename = fn;
	}
};

