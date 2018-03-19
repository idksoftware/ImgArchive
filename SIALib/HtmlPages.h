#pragma once

#include <fstream>
#include <vector>

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

class AboutHtmlPage : public BaseHtmlPage
{
	void header(std::ofstream &htmlFile);
public:
	AboutHtmlPage();
	~AboutHtmlPage();
	void content(std::ofstream &htmlFile);
};

class HomeHtmlPage : public BaseHtmlPage
{
	void header(std::ofstream &htmlFile);
public:
	HomeHtmlPage();
	~HomeHtmlPage();
	void content(std::ofstream &htmlFile);
};

class ArchiveHtmlPage : public BaseHtmlPage
{
	void header(std::ofstream &htmlFile);
public:
	ArchiveHtmlPage();
	~ArchiveHtmlPage();
	void content(std::ofstream &htmlFile);
};

class SystemHistoryFileItem {
public:
	std::string m_name;
	std::string m_start;
	std::string m_end;
	int m_numberOfItems;
};

class HistoryHtmlPage : public BaseHtmlPage
{
	std::vector<SystemHistoryFileItem> m_fileItems;
	void header(std::ofstream &htmlFile);
public:
	HistoryHtmlPage();
	~HistoryHtmlPage();
	void content(std::ofstream &htmlFile);
	void load(std::vector<SystemHistoryFileItem> &fileItems);
};

class MenuHtmlPage : public BaseHtmlPage
{
	void header(std::ofstream &htmlFile);
public:
	MenuHtmlPage();
	~MenuHtmlPage();
	void content(std::ofstream &htmlFile);
};