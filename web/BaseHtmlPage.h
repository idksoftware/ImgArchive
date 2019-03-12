#pragma once

#include <fstream>
#include <vector>
#include <memory>

namespace simplearchive {

	class HtmlPage
	{
		std::vector<std::string> m_link;
		void insertLinks(std::ofstream & htmlFile);
	protected:
		virtual void head(std::ofstream &htmlFile) {};
		virtual void header(std::ofstream &htmlFile) {};
		virtual void content(std::ofstream &htmlFile) {};
		void addLink(const char *link);
	public:
		HtmlPage();
		~HtmlPage();
		bool make(const char *path);
	};

	class BaseHtmlPage : public HtmlPage
	{
		virtual void head(std::ofstream &htmlFile);
	public:
		BaseHtmlPage();
		~BaseHtmlPage();

	};

} // namespace simplearchive