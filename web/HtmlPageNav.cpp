#include "HtmlPageNav.h"
#include <iterator>


HtmlPageNav::HtmlPageNav(int current, size_t max)
{
	m_currentPage = current;
	m_maxPages = max;
}


HtmlPageNav::~HtmlPageNav()
{
}


bool HtmlPageNav::make(std::ofstream &htmlFile) {
	bool addPrev = false;
	bool addFirst = false;
	bool addLast = false;
	bool addNext = false;
	bool makeSubSet = false;

	if ((int)size() > 1) {

		// Preconditions

		// First and Prevous selection buttons
		if (m_currentPage > 0) {
			addPrev = true;
			addFirst = true;
		}

		// Last and Next selection buttons
		if (m_currentPage < (int)size()-1) {
			addLast = true;
			addNext = true;
		}

		// Make less than all pages (Used when large number of pages generated
		if (m_maxPages < (int)size()) {
			makeSubSet = true;
		}


		// Generate Html
		htmlFile << "<ul>\n";
		int idx = 1;
		if (addFirst) {
			std::string& i = at(0);
			htmlFile << "<li><a href = \"" << i.c_str() << "\">First</a></li>\n";
		}
		if (addPrev) {
			std::string& i = at(m_currentPage-1);
			htmlFile << "<li><a href = \"" << i.c_str() << "\">Prev</a></li>\n";
		}
		if (makeSubSet) {
			int startPage = 0;
			if (m_currentPage < (int)(m_maxPages / 2)) {
				startPage = 0;
			}
			else if (m_currentPage > (int)(size() - (m_maxPages / 2))) {
				startPage = (int)(size() - m_maxPages);
			}
			for (vector<std::string>::iterator i = std::next(begin(), startPage); i != end(); i++) {
				std::string& str = *i;
				if ((m_currentPage + 1) == idx) {
					htmlFile << "<li>" << idx << "</li>\n";
				}
				else {
					htmlFile << "<li><a href = \"" << str.c_str() << "\">" << idx << "</a></li>\n";
				}
				idx++;
			}
		}
		else {
			for (auto i = begin(); i != end(); i++) {
				std::string& str = *i;
				if ((m_currentPage + 1) == idx) {
					htmlFile << "<li>" << idx << "</li>\n";
				}
				else {
					htmlFile << "<li><a href = \"" << str.c_str() << "\">" << idx << "</a></li>\n";
				}
				idx++;
			}
		}
		if (addNext) {
			std::string& i = at(m_currentPage+1);
			
			htmlFile << "<li><a href = \"" << i.c_str() << "\">Next</a></li>\n";
		}
		if (addLast) {
			std::string& i = at(size()-1);

			htmlFile << "<li><a href = \"" << i.c_str() << "\">Last</a></li>\n";
		}
		htmlFile << "</ul>\n";
	}
	return true;
}