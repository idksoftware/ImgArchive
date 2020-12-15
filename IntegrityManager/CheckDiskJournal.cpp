#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "CheckDiskJournal.h"
#include "CSVArgs.h"
#include "ExifDateTime.h"

namespace simplearchive
{
	/*
	* 14.06.12.12.16.12:56:first checkin:checkin
	*/
	CDJournalItem::CDJournalItem(const char* dataString)
	{
		CSVArgs csvArgs(':');
		csvArgs.process(dataString);
		const std::string numStr = csvArgs.at(0);
		m_num = std::stoul(numStr, nullptr, 10);
		m_image = csvArgs.at(1);
		std::string statusStr = csvArgs.at(2);
		m_status.set(statusStr.c_str());
		const std::string checkedOutStr = csvArgs.at(3);
		m_checkedOut = (checkedOutStr == "True") ? true : false;
		if (csvArgs.size() > 4)
		{
			m_orginal = csvArgs.at(4);
		}
		if (csvArgs.size() > 5)
		{
			const std::string fixedStr = csvArgs.at(5);
			m_fixed = (fixedStr == "True") ? true : false;
		}
	}

	CDJournalItem::CDJournalItem(int num, const char* image, ReportStatus& status, const char* orginal) : m_image(image)
	{
		m_num = num;
		m_status = status;
		m_checkedOut = false;
		if (orginal != nullptr)
		{
			m_orginal = orginal;
		}
		m_fixed = false;
	}

	void CDJournalItem::setCheckedOut()
	{
		m_checkedOut = true;
	}

	void CDJournalItem::setFixed()
	{
		m_fixed = true;
	}

	//bool add(const char *date, const char *version, int comment, const char *event);

	std::string CDJournalItem::toString()
	{
		std::stringstream s;
		if (m_orginal.length() != 0)
		{
			s << m_num << ":" << m_image << ":" << m_status.toString() << ":" << (m_checkedOut ? "True" : "False") <<
				":" << m_orginal;
		}
		else
		{
			s << m_num << ":" << m_image << ":" << m_status.toString() << ":" << (m_checkedOut ? "True" : "False") <<
				":";
		}
		return s.str();
	}

	void CheckDiskJournal::add(const char* image, ReportStatus& status, const char* orginal)
	{
		++m_num;
		const CDJournalItem cdJournalItem(m_num, image, status, orginal);
		m_list->push_back(cdJournalItem);
	}


	bool CheckDiskJournal::read(const char* filepath)
	{
		std::string text;
		std::ifstream file(filepath);
		if (file.is_open() == false)
		{
			return false;
		}

		while (std::getline(file, text))
		{
			CDJournalItem cdJournalItem(text.c_str());
			m_list->push_back(cdJournalItem);
		}
		file.close();

		return true;
	}

	bool CheckDiskJournal::writeXML(const char* path)
	{
		CDSummaryItem summaryItem;

		std::ofstream file(path);
		if (file.is_open() == false)
		{
			return false;
		}
		ExifDateTime timeCompleted;
		timeCompleted.now();

		file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<CheckJournal Completed=\"" << timeCompleted.toString() << "\">\n";
		file << "<List>\n";
		for (auto i = m_list->begin(); i != m_list->end(); ++i)
		{
			CDJournalItem item = *i;
			file << "\t<Item>\n";

			file << writeTag("Image", item.getImage(), 2);
			file << writeTag("Status", item.getStatus().toString(), 2);
			file << writeTag("Location", item.getOrginal(), 2);
			file << "\t</Item>\n";
			ReportStatus reportStatus = item.getStatus();
			summaryItem.update(reportStatus);
		}
		file << "</List>\n";
		file << "<Summary>\n";

		file << "<Unknown>" << summaryItem.m_unknown << "</Unknown>\n";
		file << "<ContentChanged>" << summaryItem.m_contentChanged << "</ContentChanged>\n";
		file << "<NameChanged>" << summaryItem.m_nameChanged << "</NameChanged>\n";
		file << "<Missing>" << summaryItem.m_missing << "</Missing>\n";
		file << "<Added>" << summaryItem.m_added << "</Added>\n";
		file << "<CheckedOutNoChange>" << summaryItem.m_checkedOutNoChange << "</CheckedOutNoChange>\n";
		file << "<CheckedOutChanged>" << summaryItem.m_checkedOutChanged << "</CheckedOutChanged>\n";
		file << "<Unchanged>" << summaryItem.m_unchanged << "</Unchanged>\n";

		file << "</Summary>\n";
		file << "</CheckJournal>\n";
		return true;
	}

	bool CheckDiskJournal::write(const char* path)
	{
		std::ofstream file(path);
		if (file.is_open() == false)
		{
			return false;
		}
		for (auto i = m_list->begin(); i != m_list->end(); ++i)
		{
			CDJournalItem item = *i;
			std::string line = item.toString();
			//printf("%s\n", line.c_str());
			file << line << '\n';
		}
		file.close();
		return true;
	}

	std::string CheckDiskJournal::writeTag(const char* tag, const std::string& value, int tab)
	{
		std::ostringstream xml;
		for (int i = 0; i < tab; i++)
		{
			xml << '\t';
		}
		if (!value.empty() && (value.compare("null") != 0))
		{
			xml << "<" << tag << ">" << value << "</" << tag << ">\n";
		}
		else
		{
			xml << "<" << tag << "/>\n";
		}
		return xml.str();
	}

	std::string CheckDiskJournal::writeTag(const char* tag, const int value, int tab)
	{
		std::ostringstream xml;
		for (int i = 0; i < tab; i++)
		{
			xml << '\t';
		}
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
		return xml.str();
	}

	size_t CheckDiskJournal::size()
	{
		return m_list->size();
	}

	ReportStatus& CheckDiskJournal::getReportStatusAt(int i)
	{
		return m_list->at(i).getStatus();
	}

	const char* CheckDiskJournal::getImageAt(int i)
	{
		return m_list->at(i).getImage();
	}

	const char* CheckDiskJournal::getOrginalAt(int i)
	{
		return m_list->at(i).getOrginal();
	}

	bool CheckDiskJournal::getCheckedOutState(int i)
	{
		return m_list->at(i).getCheckedOutState();
	}

	std::string CDDaySummary::toString()
	{
		std::stringstream s;
		s << m_summaryItem.m_title << ":" << m_summaryItem.m_unknown << ":" << m_summaryItem.m_contentChanged << ":" <<
			m_summaryItem.m_nameChanged << ":" << m_summaryItem.m_missing << ":" << m_summaryItem.m_added
			<< ":" << m_summaryItem.m_checkedOutNoChange << ":" << m_summaryItem.m_checkedOutChanged << ":" <<
			m_summaryItem.m_unchanged;
		return s.str();
	}

	void CDDaySummary::setStatus(ReportStatus& status)
	{
		m_summaryItem.update(status.get());
	}

	void CDYearSummary::add(const std::shared_ptr<CDDaySummary>& daySummary)
	{
		m_list->push_back(std::move(daySummary));
	}

	void CheckDiskSummaryJounal::add(const std::shared_ptr<CDYearSummary>& yearSummary)
	{
		m_list->push_back(std::move(yearSummary));
	}

	bool CheckDiskSummaryJounal::write(const char* path)
	{
		std::ofstream file(path);
		if (file.is_open() == false)
		{
			return false;
		}
		for (auto i = m_list->begin(); i != m_list->end(); ++i)
		{
			std::shared_ptr<CDYearSummary>& yitem = *i;
			CDDaySummaryList& list = yitem->getList();
			for (auto j = list.begin(); j != list.end(); ++j)
			{
				std::shared_ptr<CDDaySummary>& ditem = *j;
				std::string line = ditem->toString();
				//printf("%s\n", line.c_str());
				file << line << '\n';
			}
		}
		file.close();
		return true;
	}

	bool CheckDiskSummaryJounal::writeXML(const char* path)
	{
		CDSummaryItem totalSummary;

		std::ofstream file(path);
		if (file.is_open() == false)
		{
			return false;
		}
		ExifDateTime timeCompleted;
		timeCompleted.now();
		timeCompleted.toString();
		file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<CheckJournal Completed=\"" << timeCompleted.toString() << "\">\n";
		file << "<List>\n";

		for (auto i = m_list->begin(); i != m_list->end(); ++i)
		{
			std::shared_ptr<CDYearSummary>& yitem = *i;
			CDDaySummaryList& list = yitem->getList();
			file << "\t<Item Year=\"" << yitem->m_year << "\">\n";
			file << "\t<List>\n";
			CDSummaryItem summaryItem;
			for (auto j = list.begin(); j != list.end(); ++j)
			{
				std::shared_ptr<CDDaySummary>& ditem = *j;
				file << "\t<Item Day=\"" << ditem->m_summaryItem.m_title << "\">\n";

				file << writeTag("unknown", std::to_string(ditem->m_summaryItem.m_unknown), 2);
				file << writeTag("contentChanged", std::to_string(ditem->m_summaryItem.m_contentChanged), 2);
				file << writeTag("nameChanged", std::to_string(ditem->m_summaryItem.m_nameChanged), 2);
				file << writeTag("missing", std::to_string(ditem->m_summaryItem.m_missing), 2);
				file << writeTag("added", std::to_string(ditem->m_summaryItem.m_added), 2);
				file << writeTag("checkedOutNoChange", std::to_string(ditem->m_summaryItem.m_checkedOutNoChange), 2);
				file << writeTag("checkedOutChanged", std::to_string(ditem->m_summaryItem.m_checkedOutChanged), 2);
				file << writeTag("unchanged", std::to_string(ditem->m_summaryItem.m_unchanged), 2);
				file << "\t</Item>\n";

				summaryItem.updateAll(ditem->m_summaryItem);
			}
			file << "</List>\n";
			file << "<Summary>\n";

			file << "<Unknown>" << summaryItem.m_unknown << "</Unknown>\n";
			file << "<ContentChanged>" << summaryItem.m_contentChanged << "</ContentChanged>\n";
			file << "<NameChanged>" << summaryItem.m_nameChanged << "</NameChanged>\n";
			file << "<Missing>" << summaryItem.m_missing << "</Missing>\n";
			file << "<Added>" << summaryItem.m_added << "</Added>\n";
			file << "<CheckedOutNoChange>" << summaryItem.m_checkedOutNoChange << "</CheckedOutNoChange>\n";
			file << "<CheckedOutChanged>" << summaryItem.m_checkedOutChanged << "</CheckedOutChanged>\n";
			file << "<Unchanged>" << summaryItem.m_unchanged << "</Unchanged>\n";

			file << "</Summary>\n";
			file << "\t</Item>\n";

			totalSummary.updateAll(summaryItem);
		}
		file << "</List>\n";
		file << "<Summary>\n";

		file << "<Unknown>" << totalSummary.m_unknown << "</Unknown>\n";
		file << "<ContentChanged>" << totalSummary.m_contentChanged << "</ContentChanged>\n";
		file << "<NameChanged>" << totalSummary.m_nameChanged << "</NameChanged>\n";
		file << "<Missing>" << totalSummary.m_missing << "</Missing>\n";
		file << "<Added>" << totalSummary.m_added << "</Added>\n";
		file << "<CheckedOutNoChange>" << totalSummary.m_checkedOutNoChange << "</CheckedOutNoChange>\n";
		file << "<CheckedOutChanged>" << totalSummary.m_checkedOutChanged << "</CheckedOutChanged>\n";
		file << "<Unchanged>" << totalSummary.m_unchanged << "</Unchanged>\n";

		file << "</Summary>\n";


		file << "</CheckJournal>\n";
		file.close();
		return true;
	}

	bool CheckDiskSummaryJounal::makeSumary()
	{
		for (auto i = m_list->begin(); i != m_list->end(); ++i)
		{
			std::shared_ptr<CDYearSummary>& yitem = *i;
			CDDaySummaryList& list = yitem->getList();

			CDSummaryItem summaryItem;
			for (auto j = list.begin(); j != list.end(); ++j)
			{
				std::shared_ptr<CDDaySummary>& ditem = *j;


				summaryItem.updateAll(ditem->m_summaryItem);
			}


			m_totalSummary.updateAll(summaryItem);
		}
		return true;
	}

	std::string CheckDiskSummaryJounal::writeTag(const char* tag, const std::string& value, int tab)
	{
		std::ostringstream xml;
		for (int i = 0; i < tab; i++)
		{
			xml << '\t';
		}
		if (!value.empty() && (value.compare("null") != 0))
		{
			xml << "<" << tag << ">" << value << "</" << tag << ">\n";
		}
		else
		{
			xml << "<" << tag << "/>\n";
		}
		return xml.str();
	}

	std::string CheckDiskSummaryJounal::writeTag(const char* tag, const int value, int tab)
	{
		std::ostringstream xml;
		for (int i = 0; i < tab; i++)
		{
			xml << '\t';
		}
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
		return xml.str();
	}
}
