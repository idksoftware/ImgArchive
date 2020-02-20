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
public:
	AboutTextOut(const char* v, const char* b) : m_version(v), m_build(b) {};

	std::string writePlain();
	std::string writeXML();
	std::string writeJson();
	std::string writeHtml();
};


bool AboutCommand::process()
{
	//m_outputFile;

	AboutTextOut aboutTextOut(m_version.c_str(), m_build.c_str());
	if (aboutTextOut.parseTextOutType(m_textOutputType.c_str()) == false) {
		return false;
	}
	aboutTextOut.setFilename(m_outputFile.c_str());
	aboutTextOut.process();
	return true;
}

std::string AboutTextOut::writePlain() {

	std::stringstream str;

	str << "ImgArchive Administrator tool\n";
	str << "iaadmin version \"" << m_version << "\" (build " << m_build << ")\n";
	str << "Copyright@(2010-2016) IDK Solutions Ltd.\n";
	
	std::string s = str.str();
	return s;
}

std::string AboutTextOut::writeXML() {
	std::stringstream str;
	str << "<About>" << '\n';
	str << "\t" << writeXMLTag("Application", "iaadmin");
	str << "\t" << writeXMLTag("Description", "ImgArchive Administrator tool");
	str << "\t" << writeXMLTag("Version", m_version);
	str << "\t" << writeXMLTag("Build", m_build);
	str << "\t" << writeXMLTag("Copyright", "Copyright@(2010-2016) IDK Solutions Ltd");
	str << "</About>" << '\n';

	std::string s = str.str();
	return s;
}

std::string AboutTextOut::writeJson() {
	std::stringstream str;
	str << "{" << '\n';
	str << "\t" << writeJsonTag("Application", "iaadmin") << ",\n";
	str << "\t" << writeJsonTag("Description", "ImgArchive Administrator tool") << ",\n";
	str << "\t" << writeJsonTag("Version", m_version) << ",\n";
	str << "\t" << writeJsonTag("Build", m_build) << ",\n";
	str << "\t" << writeJsonTag("Copyright", "Copyright@(2010-2016) IDK Solutions Ltd") << "\n";
	str << "}" << '\n';

	std::string s = str.str();
	return s;
}

std::string AboutTextOut::writeHtml() {
	return std::string();
}