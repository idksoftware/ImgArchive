#pragma once
#include "CheckDisk.h"

namespace simplearchive {
	
	class CDJournalItem {

		int m_num;
		std::string m_image;
		ReportStatus m_status;
		std::string m_orginal;
		bool m_checkedOut;
		bool m_fixed;


	public:
		CDJournalItem();
		CDJournalItem(const char *dataString);
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

	class CheckDiskJournal
	{
		std::string writeTag(const char *tag, const std::string& value, int tab);
		std::string writeTag(const char *tag, const int value, int tab);
		CDJournalList *m_list;
		int m_num;
	public:
		CheckDiskJournal();
		virtual ~CheckDiskJournal();
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

};