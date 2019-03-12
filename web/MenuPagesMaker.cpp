#include "HtmlPages.h"
#include "MenuPagesMaker.h"
#include <exception>
//#include <stdio.h>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <string>
#include "ArchiveVisitor.h"
#include "pathcontroller.h"
#include "MetadataObject.h"
#include "CSVArgs.h"
#include "MenuPages.h"
#include "HtmlPages.h"
#include "HookCmd.h"

namespace simplearchive {

PageIndexMaker::PageIndexMaker()
{

}
PageIndexMaker::~PageIndexMaker() {

}


bool PageIndexMaker::process() {

	HomeHtmlPage homeHtmlPage;
	std::string fileOut = m_htmlPath;
	fileOut += "/homenav.html";
	homeHtmlPage.make(fileOut.c_str());
	ArchiveHtmlPage archiveHtmlPage;
	fileOut = m_htmlPath;
	//fileOut += "/archivenav.html";
	archiveHtmlPage.make(fileOut.c_str());
	HistoryHtmlPage historyHtmlPage;
	fileOut = m_htmlPath;
	fileOut += "/historynav.html";
	//historyHtmlPage.make(fileOut.c_str());
	JournalHtmlPage journalHtmlPage;
	fileOut = m_htmlPath;
	fileOut += "/journalnav.html";
	//journalHtmlPage.make(fileOut.c_str());
	MenuHtmlPage menuHtmlPage;
	fileOut = m_htmlPath;
	fileOut += "/menunav.html";
	menuHtmlPage.make(fileOut.c_str());
	AboutHtmlPage aboutHtmlPage;
	fileOut = m_htmlPath;
	fileOut += "/aboutnav.html";
	aboutHtmlPage.make(fileOut.c_str());
	return true;
}
/* ======================================

SystemHistoryMaker

========================================*/

SystemHistoryMaker::SystemHistoryMaker() {}
SystemHistoryMaker::~SystemHistoryMaker() {}

void contentsHeader(std::ofstream &htmlFile) {
	htmlFile << "<!DOCTYPE html>\n";
	htmlFile << "<html>\n";
	htmlFile << "<head>\n";
	htmlFile << "\t<link rel = \"stylesheet\" type = \"text/css\" href = \"../../style/history.css\">\n";
	htmlFile << "</head>\n";
	htmlFile << "<body>\n";
	htmlFile << "<h1>History Log</h1>\n";
	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Log</th>\n";
	htmlFile << "<th>Start</th>\n";
	htmlFile << "<th>End</th>\n";
	htmlFile << "<th>Number of items</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
}

void contentsFooter(std::ofstream &htmlFile) {
	htmlFile << "</tr> \n </table>\n";
	htmlFile << "</body>\n";
	htmlFile << "</html>\n";
}

bool SystemHistoryMaker::process() {
	FileList_Ptr filelist = SAUtils::getFiles_(m_historyPath.c_str());
	if (filelist == nullptr || filelist->empty() == false) {

		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string filenameItem = *i;
			if (SAUtils::getExtention(filenameItem).compare("hst") == 0) {


				m_filelist.push_back(filenameItem);
				std::string fileIn = m_historyPath;
				fileIn += '/';
				fileIn += filenameItem;

				std::string fileOut = m_htmlPath;
				fileOut += "/system/history/";
				fileOut += filenameItem;
				fileOut += ".html";
				SystemHistoryFileItem item;

				if (process(fileIn.c_str(), fileOut.c_str(), item) == false) {
					return false;
				}
				m_fileItems.push_back(item);
			}
		}
	}

	HistoryHtmlPage historyHtmlPage;
	std::string fileOut = m_htmlPath;
	fileOut += "/html/historynav.html";


	historyHtmlPage.load(m_fileItems);
	historyHtmlPage.make(fileOut.c_str());
	return true;
}



bool SystemHistoryMaker::process(const char *fileIn, const char *fileOut, SystemHistoryFileItem &item) {
	std::string text;

	std::ifstream file(fileIn);
	if (file.is_open() == false) {
		return false;
	}
	std::ofstream htmlFile(fileOut);
	if (htmlFile.is_open() == false) {
		return false;
	}
	item.m_name = SAUtils::getFilename(fileIn);

	htmlFile << "<!DOCTYPE html>\n";
	htmlFile << "<html>\n";
	htmlFile << "<head>\n";
	htmlFile << "\t<link rel = \"stylesheet\" type = \"text/css\" href = \"../../style/history.css\">\n";
	htmlFile << "</head>\n";
	htmlFile << "<body>\n";
	htmlFile << "<h1>" << SAUtils::getFilenameNoExt(fileIn) << "</h1>\n";
	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Time Date</th>\n";
	htmlFile << "<th>Image</th>\n";
	htmlFile << "<th>Version</th>\n";
	htmlFile << "<th>Arction</th>\n";
	htmlFile << "<th>Path</th>\n";
	htmlFile << "<th>Comment</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
	bool first = true;
	std::string timeDate;
	item.m_numberOfItems = 0;
	while (std::getline(file, text)) {
		//m_dataContainer->push_back(*(new std::string(text)));
		CSVArgs csvArgs(':');
		csvArgs.process(text);
		timeDate = csvArgs.at(0);
		if (first == true) {
			item.m_start = timeDate;
			first = false;
		}

		std::string path = csvArgs.at(1);
		std::string version = csvArgs.at(2);
		std::string comment = csvArgs.at(3);
		std::string action = csvArgs.at(4);

		std::string name = path.substr(16, path.length() - 16);
		path = path.substr(0, path.length() - (name.length() + 1));
		htmlFile << "<tr>\n";
		htmlFile << "<td>" << timeDate << "</td>\n";
		htmlFile << "<td>" << name << "</td>\n";
		htmlFile << "<td>" << version << "</td>\n";
		htmlFile << "<td>" << action << "</td>\n";
		htmlFile << "<td>" << path << "</td>\n";
		htmlFile << "<td>" << comment << "</td>\n";
		htmlFile << "</tr>\n";
		item.m_numberOfItems++;
	}
	item.m_end = timeDate;
	htmlFile << "</tr> \n </table>\n";

	htmlFile << "</body>\n";
	htmlFile << "</html>\n";
	htmlFile.close();
	//std::cout << config.size() << std::endl;
	file.close();
	return true;
}

ImportJournalMaker::ImportJournalMaker() {}
ImportJournalMaker::~ImportJournalMaker() {}

bool ImportJournalMaker::process(const char *fileIn, const char *fileOut, JournalFileItem &item) {
	std::string text;

	std::ifstream file(fileIn);
	if (file.is_open() == false) {
		return false;
	}
	std::ofstream htmlFile(fileOut);
	if (htmlFile.is_open() == false) {
		return false;
	}
	item.m_name = SAUtils::getFilename(fileIn);

	htmlFile << "<!DOCTYPE html>\n";
	htmlFile << "<html>\n";
	htmlFile << "<head>\n";
	htmlFile << "\t<link rel = \"stylesheet\" type = \"text/css\" href = \"../../../style/history.css\">\n";
	htmlFile << "</head>\n";
	htmlFile << "<body>\n";
	htmlFile << "<h1>" << SAUtils::getFilenameNoExt(fileIn) << "</h1>\n";
	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Source</th>\n";
	htmlFile << "<th>Action</th>\n";
	htmlFile << "<th>Path</th>\n";

	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
	bool first = true;

	item.m_complete = false;
	item.m_total = 0;
	item.m_incomplete = 0;
	item.m_imported = 0;
	item.m_duplicate = 0;
	item.m_error = 0;
	while (std::getline(file, text)) {
		//D:\tmp_raw/DSC01541.JPG,Imported,2017-01-18/DSC01541.JPG
		CSVArgs csvArgs(',');
		csvArgs.process(text);
		std::string source = csvArgs.at(0);
		std::string action = csvArgs.at(1);
		std::string path;
		if (csvArgs.size() > 2) {
			path = csvArgs.at(2);
		}
		else {
			path = "Not Importd";
		}


		htmlFile << "<tr>\n";
		htmlFile << "<td>" << source << "</td>\n";
		htmlFile << "<td>" << action << "</td>\n";
		htmlFile << "<td>" << path << "</td>\n";
		htmlFile << "</tr>\n";
		item.m_total++;
	}

	if (item.m_imported == item.m_total) {
		item.m_complete = true;
	}

	htmlFile << "</tr> \n </table>\n";

	htmlFile << "</body>\n";
	htmlFile << "</html>\n";
	htmlFile.close();
	//std::cout << config.size() << std::endl;
	file.close();
	return true;
}

bool ImportJournalMaker::process() {
	std::string importJournalPath = m_journalPath;
	importJournalPath += "/import";
	FileList_Ptr filelist = SAUtils::getFiles_(importJournalPath.c_str());
	if (filelist == nullptr || filelist->empty() == false) {

		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string filenameItem = *i;
			if (SAUtils::getExtention(filenameItem).compare("log") == 0) {


				m_filelist.push_back(filenameItem);
				std::string fileIn = importJournalPath;
				fileIn += '/';
				fileIn += filenameItem;

				std::string fileOut = m_htmlPath;
				fileOut += "/system/journal/import/";
				fileOut += filenameItem;
				fileOut += ".html";
				JournalFileItem item;
				std::string filename = SAUtils::getFilenameNoExt(filenameItem);

				std::string year = filename.substr(2, 4);
				std::string mon = filename.substr(4, 2);
				std::string day = filename.substr(6, 2);
				std::string id = filename.substr(11, 4);

				stringstream s;
				s << day << ':' << mon << ':' << year << '-' << id;
				item.m_date = s.str();

				if (process(fileIn.c_str(), fileOut.c_str(), item) == false) {
					return false;
				}
				m_fileItems.push_back(item);
			}
		}
	}

	JournalHtmlPage journalHtmlPage;
	std::string fileOut = m_htmlPath;
	fileOut += "/html/journalnav.html";

	journalHtmlPage.load(m_fileItems);
	journalHtmlPage.make(fileOut.c_str());
	return true;
}

} // namespace simplearchive
