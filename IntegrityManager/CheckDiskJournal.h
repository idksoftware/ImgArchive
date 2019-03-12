#pragma once
#include "CheckDisk.h"
#include <sstream>

namespace simplearchive {
	
	/**
		Contains file item
	*/

	class CDJournalItem {

		int m_num;
		std::string m_image;
		ReportStatus m_status;
		std::string m_orginal;
		bool m_checkedOut;
		bool m_fixed;


	public:
		CDJournalItem() {};
		explicit CDJournalItem(const char *dataString);
		CDJournalItem(int num, const char *image, ReportStatus &status, const char *orginal);
		void setFixed();
		void setCheckedOut();
		std::string toString();
		int getNumber() { return m_num; };
		const char *getImage() { return m_image.c_str(); };
		ReportStatus &getStatus() { return m_status; };
		const char *getOrginal() { return m_orginal.c_str(); };
		bool getCheckedOutState() { return m_checkedOut; };
	};

	class CDJournalList : public std::vector<CDJournalItem> {};
	/**
		Contains List of journal items.
	*/
	class CheckDiskJournal
	{
		std::string writeTag(const char *tag, const std::string& value, int tab);
		std::string writeTag(const char *tag, const int value, int tab);
		std::unique_ptr<CDJournalList> m_list;
		int m_num;
	public:
		CheckDiskJournal() : m_list(std::make_unique<CDJournalList>()), m_num(0) {}
		virtual ~CheckDiskJournal() = default;
		void add(const char *image, const char *status, const char *orginal = nullptr);
		void add(const char *image, ReportStatus &status, const char *orginal = nullptr);
		bool read(const char *filepath);
		
		bool write(const char *path);
		bool writeXML(const char *path);
		
		int size();
		ReportStatus &getReportStatusAt(int i);
		const char *getImageAt(int i);
		const char *getOrginalAt(int i);
		bool getCheckedOutState(int i);
		CDJournalList &getList() { return *m_list; }
	};

	class CDSummaryItem {
		void init() {
			m_unknown = 0;
			m_contentChanged = 0;
			m_nameChanged = 0;
			m_missing = 0;
			m_added = 0;
			m_checkedOutNoChange = 0;
			m_checkedOutChanged = 0;
			m_unchanged = 0;
		};
	public:
		int m_unknown;
		int m_contentChanged;
		int m_nameChanged;
		int m_missing;
		int m_added;
		int m_checkedOutNoChange;
		int m_checkedOutChanged;
		int m_unchanged;
		std::string m_title;
		explicit CDSummaryItem(const char *title) : m_title(title) {
			init();
		}
		CDSummaryItem() {
			init();
		}
		void update(ReportStatus reportStatus) {
			switch (reportStatus.get()) {
			case ReportStatus::Status::Unknown: m_unknown++; break;
			case ReportStatus::Status::ContentChanged: m_contentChanged++; break;
			case ReportStatus::Status::NameChanged: m_nameChanged++; break;
			case ReportStatus::Status::Missing: m_missing++; break;
			case ReportStatus::Status::Added: m_added++; break;
			case ReportStatus::Status::CheckedOutNoChange: m_checkedOutNoChange++; break;
			case ReportStatus::Status::CheckedOutChanged: m_checkedOutChanged++; break;
			case ReportStatus::Status::Unchanged: m_unchanged++; break;
			}
		}
		void updateAll(const CDSummaryItem &ditem) {
			m_unknown += ditem.m_unknown;
			m_contentChanged += ditem.m_contentChanged;
			m_nameChanged += ditem.m_nameChanged;
			m_missing += ditem.m_missing;
			m_added += ditem.m_added;
			m_checkedOutNoChange += ditem.m_checkedOutNoChange;
			m_checkedOutChanged += ditem.m_checkedOutChanged;
			m_unchanged += ditem.m_unchanged;
		}

		std::string toSummary() {
			std::stringstream str;
			str << "\tUnknown            : " << m_unknown << '\n'
				<< "\tContent Changed    : " << m_contentChanged << '\n'
				<< "\tName Changed       : " << m_nameChanged << '\n'
				<< "\tMissing            : " << m_missing << '\n'
				<< "\tAdded              : " << m_added << '\n'
				<< "\tCheckedOut NoChange: " << m_checkedOutNoChange << '\n'
				<< "\tCheckedOut Changed : " << m_checkedOutChanged << '\n'
				<< "\tUnchanged          : " << m_unchanged << '\n';
			return str.str();
		}

		std::string toResult() {
			if (m_contentChanged || m_nameChanged || m_missing) {
				return "Needs repairing";
			}
			return "No Action required";
		}
	};

	/**
		Contains day summary
	*/
	class CDDaySummary {

	public:
		CDSummaryItem m_summaryItem;
		explicit CDDaySummary(const char *day) : m_summaryItem(day) {}	
		CDDaySummary(const CDDaySummary &) = default;
		CDDaySummary& operator=(const CDDaySummary&) = default;
		virtual ~CDDaySummary() = default;
		std::string toString();
		void setStatus(ReportStatus& status);
	};

	class CDDaySummaryList : public std::vector<std::shared_ptr<CDDaySummary>> {};
		

	/**
		Contains year summary
	*/
	class CDYearSummary
	{
		std::shared_ptr<CDDaySummaryList> m_list;
		
	public:
		explicit CDYearSummary(const char *year) : m_list(std::make_shared<CDDaySummaryList>()), m_year(year) {}
		CDYearSummary(const CDYearSummary &) {};
		CDYearSummary& operator=(const CDYearSummary&) = default;
		virtual ~CDYearSummary() = default;
		
		void add(const std::shared_ptr<CDDaySummary> &daySummary);
		CDDaySummaryList &getList() { return *m_list; }
		std::string m_year;
	};

	class CDDYearSummaryList : public std::vector<std::shared_ptr<CDYearSummary>> {};

	/**
		Contains archive summary
	*/
	class CheckDiskSummaryJounal
	{
		std::string writeTag(const char *tag, const std::string& value, int tab);
		std::string writeTag(const char *tag, const int value, int tab);
		std::shared_ptr<CDDYearSummaryList> m_list;
		CDSummaryItem m_totalSummary;
		bool makeSumary();
	public:
		CheckDiskSummaryJounal() : m_list(std::make_shared<CDDYearSummaryList>()) {}
		virtual ~CheckDiskSummaryJounal() = default;
		void add(const std::shared_ptr<CDYearSummary> &yearSummary);
		bool write(const char *path);
		bool writeXML(const char *path);
		
		CDDYearSummaryList &getList() { return *m_list; }
		CDSummaryItem& getTotalSummary() {
			makeSumary();
			return m_totalSummary;
		};
	};
};