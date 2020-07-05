#include "AboutCommand.h"
//#include "AppConfig.h"
//#include "CLogger.h"
//#include "siaglobal.h"
//#include "SAUtils.h"
#include "TextOut.h"
//#include "SAUtils.h"
//#include "CSVArgs.h"
#include <iostream>
#include <sstream>

void AboutCommand::setOutputFile(const char* s)
{
	m_outputFile = s;
}

void AboutCommand::setTextOutputType(const char* s)
{
	m_textOutputType = s;
}

class AboutTextOut : public TextOut {
	
	std::string m_version;
	std::string m_build;
	std::string m_name;
	std::string m_appName;
	std::string m_platform;
public:
	AboutTextOut(const char *name, const char * appName, const char* v, const char* b)
		: m_version(v),
		  m_build(b),
		  m_name(name),
		  m_appName(appName)
	{
		
#ifdef _WIN64
		m_platform = "64 bit Windows ";
#elif _WIN32
		m_platform = "32 bit Windows";
#else
		m_platform = "64 bit Linux";
#endif
	};

	std::string writePlain();
	std::string writeXML();
	std::string writeJson();
	std::string writeHtml();
};


bool AboutCommand::process()
{
	//m_outputFile;

	AboutTextOut aboutTextOut(m_name.c_str(), m_appName.c_str(), m_version.c_str(), m_build.c_str());
	if (aboutTextOut.parseTextOutType(m_textOutputType.c_str()) == false) {
		return false;
	}
	aboutTextOut.setFilename(m_outputFile.c_str());
	aboutTextOut.process();
	return true;
}

std::string AboutTextOut::writePlain() {

	std::stringstream str;

	str << "ImgArchive " << m_name.c_str() << " tool\n";
	str << m_appName.c_str() << " version \"" << m_version << "\" (build " << m_build << ") " << __DATE__ << " " << __TIME__ << "\n";
	str << m_platform << " Copyright@(2017-2020) IDK Solutions Ltd.\n";
	
	std::string s = str.str();
	return s;
}

std::string AboutTextOut::writeXML() {
	std::string appText = "ImgArchive " + m_name + " tool";
	std::stringstream str;
	str << "<About>" << '\n';
	str << "\t" << writeXMLTag("Application", m_appName.c_str());
	str << "\t" << writeXMLTag("Description", appText.c_str());
	str << "\t" << writeXMLTag("Version", m_version);
	str << "\t" << writeXMLTag("Build", m_build);
	str << "\t" << writeXMLTag("Date", __DATE__);
	str << "\t" << writeXMLTag("Time", __TIME__);
	str << "\t" << writeXMLTag("Platform", m_platform);
	str << "\t" << writeXMLTag("Copyright", "Copyright@(2010-2016) IDK Solutions Ltd");
	str << "</About>" << '\n';

	std::string s = str.str();
	return s;
}

std::string AboutTextOut::writeJson() {
	std::string appText = "ImgArchive " + m_name + " tool";
	std::stringstream str;
	str << "{" << '\n';
	str << "\t" << writeJsonTag("Application", m_appName.c_str()) << ",\n";
	str << "\t" << writeJsonTag("Description", appText.c_str()) << ",\n";
	str << "\t" << writeJsonTag("Version", m_version) << ",\n";
	str << "\t" << writeJsonTag("Build", m_build) << ",\n";
	str << "\t" << writeJsonTag("Date", __DATE__);
	str << "\t" << writeJsonTag("Time", __TIME__);
	str << "\t" << writeJsonTag("Platform", m_platform);
	str << "\t" << writeJsonTag("Copyright", "Copyright@(2010-2016) IDK Solutions Ltd") << "\n";
	str << "}" << '\n';

	std::string s = str.str();
	return s;
}

std::string AboutTextOut::writeHtml() {
	return std::string();
}