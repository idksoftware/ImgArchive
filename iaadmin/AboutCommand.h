#pragma once
//#include "ImageExtentions.h"
//#include "TextOut.h"
#include <string>

class AboutCommand
{
	std::string m_outputFile;
	std::string m_textOutputType;
	std::string m_version;
	std::string m_build;
public:
	AboutCommand(const char* v, const char* b) : m_version(v), m_build(b) {};
	~AboutCommand() = default;
	void setOutputFile(const char* s);
	void setTextOutputType(const char* s);
	
	bool process();

};

