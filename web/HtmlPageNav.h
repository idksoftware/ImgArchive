#pragma once
#include <fstream>
#include <vector>


class HtmlPageNav : public std::vector<std::string> {
	int m_currentPage;
	size_t m_maxPages;
	std::string m_activeColor;
	std::string m_mouseOverColor;
	std::string m_inactiveColor;
public:
	HtmlPageNav(int current, size_t max);
	~HtmlPageNav();
	bool make(std::ofstream &htmlFile);
};

class HtmlYearPageNavItem {
public:
	HtmlYearPageNavItem() {};
};
class HtmlYearPageNav : public std::vector<std::string> {
	int m_currentYearPage;
	int m_maxPages;
	std::string m_activeColor;
	std::string m_mouseOverColor;
	std::string m_inactiveColor;
public:
	HtmlYearPageNav(int current, int max);
	~HtmlYearPageNav();
	bool make(std::ofstream &htmlFile);
};
