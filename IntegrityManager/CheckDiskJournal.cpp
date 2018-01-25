#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "CheckDiskJournal.h"
#include "CSVArgs.h"
#include "ExifDateTime.h"

namespace simplearchive {

	

		/*
		* 14.06.12.12.16.12:56:first checkin:checkin
		*/
	CDJournalItem::CDJournalItem(const char *dataString) {

		CSVArgs csvArgs(':');
		csvArgs.process(dataString);
		std::string numStr = csvArgs.at(0);
		m_num = std::stoul(numStr.c_str(), nullptr, 10);
		m_image = csvArgs.at(1);
		std::string statusStr = csvArgs.at(2);
		m_status.set(statusStr.c_str());
		std::string checkedOutStr = csvArgs.at(3);
		m_checkedOut = (checkedOutStr.compare("True") == 0) ? true : false;
		if (csvArgs.size() > 4) {
			m_orginal = csvArgs.at(4);
		}
		if (csvArgs.size() > 5) {
			std::string fixedStr = csvArgs.at(5);
			m_fixed = (fixedStr.compare("True") == 0) ? true : false;
		}
	}

	CDJournalItem::CDJournalItem(int num, const char *image, ReportStatus &status, const char *orginal) {
		m_num = num;
		m_image = image;
		m_status = status;
		m_checkedOut = false;
		if (orginal != nullptr) m_orginal = orginal;
		m_fixed = false;
	}

	void CDJournalItem::setCheckedOut() {
		m_checkedOut = true;
	}

	void CDJournalItem::setFixed() {
		m_fixed = true;
	}
		//bool add(const char *date, const char *version, int comment, const char *event);

	std::string CDJournalItem::toString() {
		std::stringstream s;
		if (m_orginal.length() != 0) {
			s << m_num << ":" << m_image << ":" << m_status.toString() << ":" << (m_checkedOut?"True":"False") << ":" << m_orginal;
		}
		else {
			s << m_num << ":" << m_image << ":" << m_status.toString() << ":" << (m_checkedOut?"True":"False") << ":";
		}
		return s.str();
	}

		

	

	

	CheckDiskJournal::CheckDiskJournal()
	{
		m_num = 0;
		m_list = new CDJournalList;
	}

	

	CheckDiskJournal::~CheckDiskJournal()
	{
		delete m_list;
	}

	void CheckDiskJournal::add(const char *image, ReportStatus &status, const char *orginal) {
		++m_num;
		CDJournalItem cdJournalItem(m_num, image, status, orginal);
		m_list->push_back(cdJournalItem);
	}



	bool CheckDiskJournal::read(const char *filepath) {
	std::string text;
		std::ifstream file(filepath);
		if (file.is_open() == false) {
			return false;
		}

		while (std::getline(file, text)) {
			CDJournalItem cdJournalItem(text.c_str());
			m_list->push_back(cdJournalItem);
		}
		file.close();

		return true;
	}
	
	bool CheckDiskJournal::writeXML(const char *path) {
		std::ofstream file(path);
		if (file.is_open() == false) {
			return false;
		}
		ExifDateTime timeCompleted;
		timeCompleted.now();
		timeCompleted.toString();
		file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<ImportJournal Completed=\"" << timeCompleted.toString() << "\">\n";

		for (std::vector<CDJournalItem>::iterator i = m_list->begin(); i != m_list->end(); i++) {
			CDJournalItem item = *i;
			file << "\t<Event>\n";

			file << writeTag("Image", item.getImage(), 2);
			file << writeTag("Status", item.getStatus().toString(), 2);
			file << writeTag("Location", item.getOrginal(), 2);
			file << "\t</Event>\n";
		}
		file << "</Catalog>\n";
		return true;
	}

	bool CheckDiskJournal::write(const char *path) {
		std::ofstream file(path);
		if (file.is_open() == false) {
			return false;
		}
		for (std::vector<CDJournalItem>::iterator i = m_list->begin(); i != m_list->end(); i++) {
			CDJournalItem item = *i;
			std::string line = item.toString();
			//printf("%s\n", line.c_str());
			file << line << '\n';

		}
		file.close();
		return true;
	}

	std::string CheckDiskJournal::writeTag(const char *tag, const std::string& value, int tab) {
		std::ostringstream xml;
		for (int i = 0; i < tab; i++) {
			xml << '\t';
		}
		if (!value.empty() && (value.compare("null") != 0)) {
			xml << "<" << tag << ">" << value << "</" << tag << ">\n";
		}
		else {
			xml << "<" << tag << "/>\n";
		}
		return xml.str();
	}

	std::string CheckDiskJournal::writeTag(const char *tag, const int value, int tab) {
		std::ostringstream xml;
		for (int i = 0; i < tab; i++) {
			xml << '\t';
		}
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
		return xml.str();
	}

	int CheckDiskJournal::size()
	{
		return m_list->size();
	}

	ReportStatus &CheckDiskJournal::getReportStatusAt(int i)
	{
		return m_list->at(i).getStatus();
	}

	const char *CheckDiskJournal::getImageAt(int i)
	{
		return m_list->at(i).getImage();
	}

	const char *CheckDiskJournal::getOrginalAt(int i)
	{
		return m_list->at(i).getOrginal();
	}

	bool CheckDiskJournal::getCheckedOutState(int i) {
		return m_list->at(i).getCheckedOutState();
	}
}
