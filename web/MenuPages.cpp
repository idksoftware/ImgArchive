#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include "BaseHtmlPage.h"
#include "MenuPages.h"
#include "SAUtils.h"

namespace simplearchive {

AboutHtmlPage::AboutHtmlPage()
{
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
}

AboutHtmlPage::~AboutHtmlPage()
{
}

void AboutHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a class = \"active\" href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void AboutHtmlPage::content(std::ofstream &htmlFile) {
	/*
	htmlFile << "<!DOCTYPE html>\n"\
	"	<html>\n"\
	"	<head>\n"\
	"	<meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\">\n"\
	"	<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">\n"\
	"	</head>\n"\
	"	<body>\n"\
	"	<ul class = \"topnav\">\n"\
	"	<li><a href = \"homenav.html\">Home</a></li>\n"\
	"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
	"	<li><a href = \"historynav.html\">History</a></li>\n"\
	"	<li class = \"right\"><a class = \"active\" href = \"aboutnav.html\">About</a></li>\n"\
	"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
	"	</ul>\n"\
	"	<div style = \"padding:0 16px;\">\n"\
	"	<h2>About page</h2>\n"\
	"</div>\n"\
	"</body>\n"\
	"</html>\n";
	*/
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>About page</h2>\n"\
		"</div>\n";
}

HomeHtmlPage::HomeHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
}

HomeHtmlPage::~HomeHtmlPage() {
}

void HomeHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Home page</h2>\n"\
		"	</div>\n";
}

void HomeHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a  class = \"active\" href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}




ArchiveHtmlPage::ArchiveHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/history.css\">");
}

ArchiveHtmlPage::~ArchiveHtmlPage() {
}

void ArchiveHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a class = \"active\" href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void ArchiveHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Archive page</h2>\n"\
		"	</div>\n";

	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Year</th>\n";

	htmlFile << "<th>Number of days</th>\n";
	htmlFile << "<th>Number of images</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
	for (auto i = m_fileItems.begin(); i != m_fileItems.end(); i++) {
		YearFileItem item = *i;
		htmlFile << "<tr>\n";
		htmlFile << "<td><a href=\"" << "../images/" << item.m_year << '/' << item.m_firstDay << "/html/" << item.m_firstDay << ".html\">" << item.m_year << "</td>\n";
		htmlFile << "<td>" << item.m_numberOfDays << "</td>\n";
		htmlFile << "<td>" << item.m_numberOfImages << "</td>\n";
		htmlFile << "</tr>\n";
	}
	htmlFile << "</tr> \n </table>\n";
}

void ArchiveHtmlPage::load(std::vector<YearFileItem>& fileItems)
{
	for (auto i = fileItems.begin(); i != fileItems.end(); i++) {
		m_fileItems.push_back(*i);
	}
}



JournalHtmlPage::JournalHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/history.css\">");
}

JournalHtmlPage::~JournalHtmlPage() {
}

void JournalHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a class = \"active\" href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void JournalHtmlPage::load(std::vector<JournalFileItem> &fileItems) {
	for (auto i = fileItems.begin(); i != fileItems.end(); i++) {
		m_fileItems.push_back(*i);
	}
}
/*
std::string m_date;
std::string m_name;
bool m_complete;
int m_total;
int m_incomplete;
int m_imported;
int m_duplicate;
int m_error;
*/
void JournalHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Import journal page</h2>\n"\
		"	</div>\n";
	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Date</th>\n";
	htmlFile << "<th>Name/ID</th>\n";
	htmlFile << "<th>Complete</th>\n";
	htmlFile << "<th>Total</th>\n";
	htmlFile << "<th>Incomplete</th>\n";
	htmlFile << "<th>Imported</th>\n";
	htmlFile << "<th>Duplicate</th>\n";
	htmlFile << "<th>Error</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
	for (auto i = m_fileItems.begin(); i != m_fileItems.end(); i++) {
		JournalFileItem item = *i;
		htmlFile << "<tr>\n";
		htmlFile << "<td><a href=\"" << "../system/journal/import/" << item.m_name << ".html\">" << SAUtils::getFilenameNoExt(item.m_name) << "</td>\n";
		htmlFile << "<td>" << item.m_date << "</td>\n";
		htmlFile << "<td>" << item.m_name << "</td>\n";
		htmlFile << "<td>" << item.m_complete << "</td>\n";
		htmlFile << "<td>" << item.m_total << "</td>\n";
		htmlFile << "<td>" << item.m_incomplete << "</td>\n";
		htmlFile << "<td>" << item.m_imported << "</td>\n";
		htmlFile << "<td>" << item.m_duplicate << "</td>\n";
		htmlFile << "<td>" << item.m_error << "</td>\n";
		htmlFile << "</tr>\n";
	}
	htmlFile << "</tr> \n </table>\n";
}

HistoryHtmlPage::HistoryHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/history.css\">");
}

HistoryHtmlPage::~HistoryHtmlPage() {
}

void HistoryHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a class = \"active\" href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void HistoryHtmlPage::load(std::vector<SystemHistoryFileItem> &fileItems) {
	for (auto i = fileItems.begin(); i != fileItems.end(); i++) {
		m_fileItems.push_back(*i);
	}
}

void HistoryHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>History page</h2>\n"\
		"	</div>\n";
	htmlFile << "<table> \n <tr>\n";
	htmlFile << "<th>Log</th>\n";
	htmlFile << "<th>Start</th>\n";
	htmlFile << "<th>End</th>\n";
	htmlFile << "<th>Number of items</th>\n";
	htmlFile << "</tr>\n";
	htmlFile << "<tr>\n";
	for (auto i = m_fileItems.begin(); i != m_fileItems.end(); i++) {
		SystemHistoryFileItem item = *i;
		htmlFile << "<tr>\n";
		htmlFile << "<td><a href=\"" << "../system/history/" << item.m_name << ".html\">" << SAUtils::getFilenameNoExt(item.m_name) << "</td>\n";
		htmlFile << "<td>" << item.m_start << "</td>\n";
		htmlFile << "<td>" << item.m_end << "</td>\n";
		htmlFile << "<td>" << item.m_numberOfItems << "</td>\n";
		htmlFile << "</tr>\n";
	}
	htmlFile << "</tr> \n </table>\n";
}



MenuHtmlPage::MenuHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
}

MenuHtmlPage::~MenuHtmlPage() {
}

void MenuHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li><a href = \"journalnav.html\">Journal</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a class = \"active\" href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void MenuHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Menu page</h2>\n"\
		"	</div>\n";
}

} //namespace simplearchive