#include "BaseHtmlPage.h"

namespace simplearchive {

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

} // namespace simplearchive