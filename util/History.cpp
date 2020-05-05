#include "History.h"

#include <memory>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "HistoryEvent.h"
#include "ImageHistory.h"
#include "SystemHistory.h"
#include "ChangeLog.h"
#include "ErrorCode.h"
#include "CSVArgs.h"
#include "LogDocument.h"

namespace simplearchive {

	ImageHistorySchema ImageHistoryRow::m_tableSchema;
	ImageHistoryRow::ImageHistoryRow() : MTRow(m_tableSchema) {}
	ImageHistoryRow::ImageHistoryRow(const char *filepath, const char *version, const char *comment, const HistoryEvent &he) : MTRow(m_tableSchema) {
		ExifDateTime date;
		date.now();
		columnAt(static_cast<int>(ImageHistoryIndex::IH_FILEPATH_IDX)).fromString(filepath);;
		columnAt(static_cast<int>(ImageHistoryIndex::IH_DATEADDED_IDX)).fromString(date.toString());;
		columnAt(static_cast<int>(ImageHistoryIndex::IH_VERSION_IDX)).fromString(version);;
		columnAt(static_cast<int>(ImageHistoryIndex::IH_EVENT_IDX)).fromString(he.getString());;
		columnAt(static_cast<int>(ImageHistoryIndex::IH_COMMENT_IDX)).fromString(comment);;
	}



	SystemHistorySchema SystemHistoryRow::m_tableSchema;
	SystemHistoryRow::SystemHistoryRow() : MTRow(m_tableSchema) {}
	SystemHistoryRow::SystemHistoryRow(const char *filepath, const char *version, const char *comment, const HistoryEvent &he) : MTRow(m_tableSchema) {
		ExifDateTime date;
		date.now();
		columnAt(static_cast<int>(SystemHistoryIndex::SH_DATEADDED_IDX)).fromString(date.toString());;
		columnAt(static_cast<int>(SystemHistoryIndex::SH_FILEPATH_IDX)).fromString(filepath);;
		columnAt(static_cast<int>(SystemHistoryIndex::SH_VERSION_IDX)).fromString(version);;
		columnAt(static_cast<int>(SystemHistoryIndex::SH_EVENT_IDX)).fromString(he.getString());;
		columnAt(static_cast<int>(SystemHistoryIndex::SH_COMMENT_IDX)).fromString(comment);;
	}

	std::unique_ptr<History> History::m_this = nullptr;
	std::once_flag History::m_onceFlag;
	std::string History::m_indexPath;
	std::string History::m_workspacePath;
	std::string History::m_systemHisteryPath;
	std::string History::m_changeLogPath;

	History::History() : m_systemHistory(std::make_unique<SystemHistory>()),
						m_imageHistory(std::make_unique<ImageHistory>()),
						m_changeLog(std::make_unique<ChangeLog>())
	{
		
	}


	History::~History()
	{
	}

	bool History::init() {
		if (m_systemHistory->init() == false) {
			return false;
		}
		if (m_imageHistory->init() == false) {
			return false;
		}
		
		return true;
	}

	History &History::getHistory() {
		std::call_once(m_onceFlag,
			[] {
			if (m_this == nullptr) {
				m_this = std::make_unique<History>();
			}
		});
		return *(m_this);

		
	}

	void History::setPaths(const char *index, const char *workspace, const char *system) {
		m_indexPath = index;
		m_workspacePath = workspace;
		m_systemHisteryPath = system;
		SystemHistory::setPath(system, index);
		ImageHistory::setPath(workspace, index);
	}

	bool History::newImage(const char *filepath, const char *comment) {
		
		if (m_systemHistory->add(filepath, "0000", comment, HistoryEvent::Event::ADDED) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
	
		if (m_imageHistory->add(filepath, "0000", comment, HistoryEvent::Event::ADDED) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		
		return true;
	}

	std::string Version2String(int version) {
		std::stringstream s;
		s << std::setw(4) << std::setfill('0') << version;
		return s.str();
	}
	


	bool History::checkinImage(const char *filepath, int version, const char *comment) { 
		std::string versionString = Version2String(version);

		if (m_systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKIN) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (m_imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKIN) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}

	bool History::checkoutImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (m_systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (m_imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
		
	}

	bool History::uncheckoutImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (m_systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::UNCHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (m_imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::UNCHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}

	bool History::exportImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (m_systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::EXPORT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (m_imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::EXPORT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}

	bool History::logImageHistory(const char *imagepath, LogDocument::FormatType formatType, const char* filepath) {
		
		if (imagepath == nullptr || imagepath[0] == '\0') {
			return false;
		}

		std::shared_ptr<ImageHistoryLog> log;
		if ((log = m_imageHistory->getEntries(imagepath)) == nullptr) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		log->setFilename(filepath);
		if (!log->write(formatType)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}

	bool History::logSystemHistory(const char* from, const char* to, LogDocument::FormatType formatType, const char* filepath)
	{
		ExifDateTime dateFrom;
		ExifDateTime dateTo;

		if (from != nullptr || from[0] != '\0') {
			dateFrom.fromString(from);
		}
		else {
			// start of logging
		}
		if (to != nullptr || to[0] != '\0') {
			dateTo.fromString(to);
		}
		else {
			// start of logging
		}
		std::shared_ptr<SystemHistoryLog> log;
		if ((log = m_systemHistory->getEntries()) == nullptr) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}


	SystemHistoryLog::SystemHistoryLog() : LogDocument(std::make_shared<SystemHistorySchema>()) {
		// TODO Auto-generated constructor stub

	}

	SystemHistoryLog::~SystemHistoryLog() {
		// TODO Auto-generated destructor stub
	}

	bool SystemHistoryLog::writeHuman() {
		std::ofstream file;
		if (!m_filename.empty()) {
			file.open(m_filename.c_str());
			if (file.is_open() == false) {
				return false;
			}
			file << "\n---------------------------------------------------\n";
			file << "Image: " << m_title << '\n';
			file << "Path : " << m_description << '\n';
			file << "=====================================================\n";
			file << "Date Time             version     Event      Comment\n\n";
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}

				file << csvArgs.at(0) << "    ";
				file << csvArgs.at(1) << "      ";
				file << csvArgs.at(4) << "  ";
				file << csvArgs.at(3) << "\n\n";
			}
		}
		else {
			
			std::cout << "\n---------------------------------------------------\n";
			std::cout << "Image: " << m_title << '\n';
			std::cout << "Path : " << m_description << '\n';
			std::cout << "=====================================================\n";
			std::cout << "Date Time             version     Event      Comment\n\n";
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}

				std::cout << csvArgs.at(0) << "    ";
				std::cout << csvArgs.at(1) << "      ";
				std::cout << csvArgs.at(4) << "  ";
				std::cout << csvArgs.at(3) << "\n\n";
			}
		}
		return true;
		return true;
	}

	bool SystemHistoryLog::writeJson() {
		return true;
	}

	bool SystemHistoryLog::writeCSV() {
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			std::cout << *i << '\n';
		}
		return true;
	}

	bool SystemHistoryLog::writeXML() {
		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			//std::cout << *i << '\n';
			CSVArgs csvArgs(':');
			if (csvArgs.process(i->c_str()) == false) {
				return false;
			}
			std::cout << "\t<Event>\n";
			std::cout << writeTag("DateTime", csvArgs.at(0), 2);
			std::cout << writeTag("ImagePage", csvArgs.at(1), 2);
			std::cout << writeTag("Value", csvArgs.at(2), 2);
			std::cout << writeTag("Comment", csvArgs.at(3), 2);
			std::cout << writeTag("Event", csvArgs.at(4), 2);
			std::cout << "\t</Event>\n";
		}
		std::cout << "</Catalog>\n";
		return true;
	}

	bool SystemHistoryLog::writeHtml() {
		return true;
	}
	
	ImageHistoryLog::ImageHistoryLog() : LogDocument(std::make_shared<ImageHistorySchema>()) {
		// TODO Auto-generated constructor stub

	}

	ImageHistoryLog::~ImageHistoryLog() {
		// TODO Auto-generated destructor stub
	}

	bool ImageHistoryLog::writeHuman() {
		std::ofstream file;
		if (!m_filename.empty()) {
			file.open(m_filename.c_str());
			if (file.is_open() == false) {
				return false;
			}
			file << "\n---------------------------------------------------\n";
			file << "Image: " << m_title << '\n';
			file << "Path : " << m_description << '\n';
			file << "=====================================================\n";
			file << "Date Time             version     Event      Comment\n\n";
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}

				file << csvArgs.at(0) << "    ";
				file << csvArgs.at(1) << "      ";
				file << csvArgs.at(4) << "  ";
				file << csvArgs.at(3) << "\n\n";
			}
		}
		else {
			
			std::cout << "\n---------------------------------------------------\n";
			std::cout << "Image: " << m_title << '\n';
			std::cout << "Path : " << m_description << '\n';
			std::cout << "=====================================================\n";
			std::cout << "Date Time             version     Event      Comment\n\n";
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}

				std::cout << csvArgs.at(0) << "    ";
				std::cout << csvArgs.at(1) << "      ";
				std::cout << csvArgs.at(4) << "  ";
				std::cout << csvArgs.at(3) << "\n\n";
			}
		}
		return true;
	}
	
	bool ImageHistoryLog::writeJson() {
		std::ofstream file;
		if (!m_filename.empty()) {
			file.open(m_filename.c_str());
			if (file.is_open() == false) {
				return false;
			}
			file << "{\n\"name\":\"" << m_title << "\",\n";
			file << "\"path\":\"" << m_description << "\",\n";
			file << "\"History\": [\n";
			bool first = true;
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//file << *i << '\n';

				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}
				if (first) first = false; else file << ",\n";
				file << "{";
				file << "\"DateTime\":\"" << csvArgs.at(0) << "\",";
				file << "\"Version\":\"" << csvArgs.at(1) << "\",";
				file << "\"Event\":\"" << csvArgs.at(4) << "\",";
				file << "\"Comment\":\"" << csvArgs.at(3) << "\"";
				file << "}";
			}
			file << "\n]}\n";
			file.close();
		}
		else {
			std::cout << "{\n\"name\":\"" << m_title << "\",\n";
			std::cout << "\"path\":\"" << m_description << "\",\n";
			std::cout << "\"History\": [\n";
			bool first = true;
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//file << *i << '\n';

				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}
				if (first) first = false; else file << ",\n";
				std::cout << "{";
				std::cout << "\"DateTime\":\"" << csvArgs.at(0) << "\",";
				std::cout << "\"Version\":\"" << csvArgs.at(1) << "\",";
				std::cout << "\"Event\":\"" << csvArgs.at(4) << "\",";
				std::cout << "\"Comment\":\"" << csvArgs.at(3) << "\"";
				std::cout << "}";
			}
			std::cout << "\n]}\n";
		}
		return true;
	}

	bool ImageHistoryLog::writeCSV() {
		std::ofstream file;
		if (!m_filename.empty()) {
			file.open(m_filename.c_str());
			if (file.is_open() == false) {
				return false;
			}
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				file << *i << '\n';
			}
		}
		else {
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				std::cout << *i << '\n';
			}
		}
		return true;
	}

	bool ImageHistoryLog::writeXML() {

		std::ofstream file;
		if (!m_filename.empty()) {
			file.open(m_filename.c_str());
			if (file.is_open() == false) {
				return false;
			}
			file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
				<< "<History Image=\"" << m_title << "\" Path=\"" << m_description << "\">\n";

			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}
				file << "\t<Event>\n";
				file << writeTag("DateTime", csvArgs.at(0), 2);
				file << writeTag("Version", csvArgs.at(1), 2);
				file << writeTag("Event", csvArgs.at(4), 2);
				file << writeTag("Comment", csvArgs.at(3), 2);
				file << "\t</Event>\n";
			}
			file << "</History>\n";
			file.close();
			return true;
		}
		else {
			std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
				<< "<History Image=\"" << m_title << "\" Path=\"" << m_description << "\">\n";

			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}
				std::cout << "\t<Event>\n";
				std::cout << writeTag("DateTime", csvArgs.at(0), 2);
				std::cout << writeTag("Version", csvArgs.at(1), 2);
				std::cout << writeTag("Event", csvArgs.at(4), 2);
				std::cout << writeTag("Comment", csvArgs.at(3), 2);
				std::cout << "\t</Event>\n";
			}
			std::cout << "</History>\n";
			return true;
		}
	}

	bool ImageHistoryLog::writeHtml() {

		std::ofstream file;
		if (!m_filename.empty()) {
			file.open(m_filename.c_str());
			if (file.is_open() == false) {
				return false;
			}
			file << "<!DOCTYPE html><html><head><style>\n";
			file << "table{ font-family: arial, sans-serif; border-collapse: collapse; width: 100 %;}\n";
			file << "td, th{ border: 1px solid #dddddd; text-align: left; padding: 8px;}\n";
			file << "tr:nth-child(even) {background-color: #dddddd;}\n";
			file << "</style></head><body>\n";

			file << "<h2>" << m_title << "</h2>\n";
			file << "<table><tr><td>Path:</td><td>\n";
			file << m_description;	
			file << "</td><tr></table>\n";

			file << "<table>\n";
			file << "<tr><th>Date/Time</th><th>Version</th><th>Action</th><th>Comment</th></tr>";
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}
				file << "<tr>\n";
				file << "<td>" << csvArgs.at(0) << "</td>\n";
				file << "<td>" << csvArgs.at(1) << "</td>\n";
				file << "<td>" << csvArgs.at(4) << "</td>\n";
				file << "<td>" << csvArgs.at(3) << "</td>\n"; 
				file << "</tr>\n";
			}
			file << "</table></body></html>\n";
			file.close();
		}
		else {
			std::cout << "<!DOCTYPE html><html><head><style>\n";
			std::cout << "table{ font-family: arial, sans-serif; border-collapse: collapse; width: 100 %;}\n";
			std::cout << "td, th{ border: 1px solid #dddddd; text-align: left; padding: 8px;}\n";
			std::cout << "tr:nth-child(even) {background-color: #dddddd;}\n";
			std::cout << "</style></head><body>\n";

			std::cout << "<h2>HTML Table</h2>\n";

			std::cout << "<table>\n";
			std::cout << "<tr><th>Date/Time</th><th>Version</th><th>Action</th><th>Comment</th></tr>";
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}
				std::cout << "<tr>\n";
				std::cout << "<td>" << csvArgs.at(0) << "</td>\n";
				std::cout << "<td>" << csvArgs.at(1) << "</td>\n";
				std::cout << "<td>" << csvArgs.at(4) << "</td>\n";
				std::cout << "<td>" << csvArgs.at(3) << "</td>\n"; std::cout << "</tr>\n";
			}
			std::cout << "</table></body></html>\n";
		}
			return true;
	}
	
	

} //namespace simplearchive

