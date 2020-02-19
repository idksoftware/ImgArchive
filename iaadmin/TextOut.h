#pragma once
#include <string>

enum class TextOutType {
	plain,
	xml,
	json,
	html,
	unknown
};

#define XML_HEATER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

class TextOut
{	
protected:
	std::string m_filename;
	std::string writePlainTag(const char* tag, const std::string& value);
	std::string writeXMLTag(const char* tag, const std::string& value);
	std::string writeJsonTag(const char* tag, const std::string& value);
	std::string writeHtmlTag(const char* tag, const std::string& value);

	std::string TextOut::writeXMLTag(const char* tag, bool b);

	std::string writePlainTag(const char* tag, const char* value);
	std::string writeXMLTag(const char* tag, const char* value);
	std::string writeJsonTag(const char* tag, const char* value);
	std::string writeHtmlTag(const char* tag, const char* value);
	TextOutType m_textOutType;
public:
	TextOut() : m_textOutType(TextOutType::plain) {};
	virtual ~TextOut() = default;
	void setFilename(const char *fn) {
		m_filename = fn;
	}
	bool parseTextOutType(const char* s);
	std::string TextOut::process();
	virtual std::string writePlain() = 0;
	virtual std::string writeXML() = 0;
	virtual std::string writeJson() = 0;
	virtual std::string writeHtml() = 0;
};

