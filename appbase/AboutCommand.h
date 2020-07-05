#pragma once

#include <string>

class AboutCommand
{
	std::string m_outputFile;
	std::string m_textOutputType;
	std::string m_version;
	std::string m_build;
	std::string m_name;
	std::string m_appName;
public:
	AboutCommand(const char* name, const char* appName, const char* v, const char* b)
		: m_version(v),
		  m_build(b),
		  m_name(name),
		  m_appName(appName)
	{};
	~AboutCommand() = default;
	void setOutputFile(const char* s);
	void setTextOutputType(const char* s);
	
	bool process();

};

