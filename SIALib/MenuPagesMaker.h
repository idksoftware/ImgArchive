#pragma once
#include <string>
#include <vector>
//#include "MasterCatalogue.h"
#include "SAUtils.h"


namespace simplearchive {

class SystemHistoryFileItem;
class JournalFileItem;

class PageIndexMaker
{
	std::string m_templatePath;
	std::string m_htmlPath;

public:
	PageIndexMaker();
	~PageIndexMaker();
	bool setTemplatePath(const char *templatePath, const char* htmlPath) {
		m_templatePath = templatePath;
		if (SAUtils::DirExists(templatePath) == false) {
			return false;
		}
		m_htmlPath = htmlPath;
		return true;

	}

	bool process();
};


class SystemHistoryMaker
{
	std::vector<SystemHistoryFileItem> m_fileItems;
	std::string m_templatePath;
	std::string m_htmlPath;
	std::string m_historyPath;
	std::vector<std::string> m_filelist;
	bool process(const char *fileIn, const char *fileOut, SystemHistoryFileItem &item);
public:
	SystemHistoryMaker();
	~SystemHistoryMaker();
	bool setPaths(const char *templatePath, const char* htmlPath, const char* historyPath) {
		m_templatePath = templatePath;
		if (SAUtils::DirExists(templatePath) == false) {
			return false;
		}
		m_htmlPath = htmlPath;
		m_historyPath = historyPath;
		return true;

	}
	bool process();
};

class ImportJournalMaker
{
	std::vector<JournalFileItem> m_fileItems;
	std::string m_templatePath;
	std::string m_htmlPath;
	std::string m_journalPath;
	std::vector<std::string> m_filelist;
	bool process(const char *fileIn, const char *fileOut, JournalFileItem &item);
public:
	ImportJournalMaker();
	~ImportJournalMaker();
	bool setPaths(const char *templatePath, const char* htmlPath, const char* journalPath) {
		m_templatePath = templatePath;
		if (SAUtils::DirExists(templatePath) == false) {
			return false;
		}
		m_htmlPath = htmlPath;
		m_journalPath = journalPath;
		return true;

	}
	bool process();
};

} // namespace simplearchive
