#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include "BaseHtmlPage.h"

namespace simplearchive {

	class AboutHtmlPage : public BaseHtmlPage
	{
		void header(std::ofstream &htmlFile);
	public:
		AboutHtmlPage();
		virtual ~AboutHtmlPage();
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

	class YearFileItem {
	public:
		std::string m_year;
		std::string m_firstDay;
		size_t m_numberOfDays;
		size_t m_numberOfImages;
	};

	class ArchiveHtmlPage : public BaseHtmlPage
	{
		void header(std::ofstream &htmlFile);
		std::vector<YearFileItem> m_fileItems;
	public:
		ArchiveHtmlPage();
		~ArchiveHtmlPage();
		void content(std::ofstream &htmlFile);
		void load(std::vector<YearFileItem> &fileItems);
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
		virtual ~HistoryHtmlPage();
		void content(std::ofstream &htmlFile);
		void load(std::vector<SystemHistoryFileItem> &fileItems);
	};

	class JournalFileItem {
	public:
		std::string m_date;
		std::string m_name;
		bool m_complete;
		int m_total;
		int m_incomplete;
		int m_imported;
		int m_duplicate;
		int m_error;
	};

	class JournalHtmlPage : public BaseHtmlPage
	{
		std::vector<JournalFileItem> m_fileItems;
		void header(std::ofstream &htmlFile);
	public:
		JournalHtmlPage();
		~JournalHtmlPage();
		void content(std::ofstream &htmlFile);
		void load(std::vector<JournalFileItem> &fileItems);
	};

	class MenuHtmlPage : public BaseHtmlPage
	{
		void header(std::ofstream &htmlFile);
	public:
		MenuHtmlPage();
		~MenuHtmlPage();
		void content(std::ofstream &htmlFile);
	};

} // namespace simplearchive
