#include "HtmlPages.h"
#include "SAUtils.h"

HtmlPage::HtmlPage()
{
}


HtmlPage::~HtmlPage()
{
}

//void HtmlPage::content(std::ofstream & htmlFile)
//{
//}

void HtmlPage::addLink(const char *link)
{
	m_link.push_back(link);
}

void HtmlPage::insertLinks(std::ofstream & htmlFile)
{
	for (auto ii = m_link.begin(); ii != m_link.end(); ii++) {
		std::string& link = *ii;
		htmlFile << '\t' << link.c_str() << '\n';
	}
}


bool HtmlPage::make(const char *path) {
	std::ofstream htmlFile(path);
	if (htmlFile.is_open() == false) {
		return false;
	}
	htmlFile << "<!DOCTYPE html>\n";
	htmlFile << "<html>\n";
	htmlFile << "<head>\n";
	htmlFile << "	<meta name = \"viewport\" content = \"width=device-width, initial-scale=1.0\">\n";
	insertLinks(htmlFile);
	head(htmlFile);
	
	htmlFile << "</head>\n";
	htmlFile << "<body>\n";
	header(htmlFile);
	content(htmlFile);
	htmlFile << "</body>\n";
	htmlFile << "</html>\n";
	htmlFile.close();
	return true;
}



BaseHtmlPage::BaseHtmlPage()
{
}

BaseHtmlPage::~BaseHtmlPage()
{
}

void BaseHtmlPage::head(std::ofstream & htmlFile)
{
}


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
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}




ArchiveHtmlPage::ArchiveHtmlPage() {
	addLink("<link rel = \"stylesheet\" type = \"text/css\" href = \"../style/mainnav.css\">");
}

ArchiveHtmlPage::~ArchiveHtmlPage() {
}

void ArchiveHtmlPage::header(std::ofstream &htmlFile) {
	htmlFile << "	<ul class = \"topnav\">\n"\
		"	<li><a href = \"homenav.html\">Home</a></li>\n"\
		"	<li><a class = \"active\" href = \"archivenav.html\">Archive</a></li>\n"\
		"	<li><a href = \"historynav.html\">History</a></li>\n"\
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void ArchiveHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Archive page</h2>\n"\
		"	</div>\n";
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
		"	<li class = \"right\"><a href = \"aboutnav.html\">About</a></li>\n"\
		"	<li class = \"right\"><a class = \"active\" href = \"menunav.html\">&#9776; </a></li>\n"\
		"	</ul>\n";
}

void MenuHtmlPage::content(std::ofstream &htmlFile) {
	htmlFile << "	<div style = \"padding:0 16px;\">\n"\
		"	<h2>Menu page</h2>\n"\
		"	</div>\n";
}

