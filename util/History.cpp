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

	History::History() : systemHistory(std::make_unique<SystemHistory>()),
						imageHistory(std::make_unique<ImageHistory>()),
						changeLog(std::make_unique<ChangeLog>())
	{
		
	}


	History::~History()
	{
	}

	bool History::init() {
		if (systemHistory->init() == false) {
			return false;
		}
		if (imageHistory->init() == false) {
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
		
		if (systemHistory->add(filepath, "0000", comment, HistoryEvent::Event::ADDED) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
	
		if (imageHistory->add(filepath, "0000", comment, HistoryEvent::Event::ADDED) == false) {
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

		if (systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKIN) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKIN) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}

	bool History::checkoutImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
		
	}

	bool History::uncheckoutImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::UNCHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::UNCHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}

	bool History::exportImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::EXPORT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::EXPORT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}

	bool History::logImageHistory(const char *filepath, LogDocument::FormatType formatType) {
		std::shared_ptr<ImageHistoryLog> log;
		if ((log = imageHistory->getEntries(filepath)) == nullptr) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		
		if (!log->write(formatType)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}


	HistoryLog::HistoryLog() : LogDocument(std::make_shared<ImageHistorySchema>()) {
		// TODO Auto-generated constructor stub

	}

	HistoryLog::~HistoryLog() {
		// TODO Auto-generated destructor stub
	}

	bool HistoryLog::writeHuman() {
		return true;
	}

	bool HistoryLog::writeJson() {
		return true;
	}

	bool HistoryLog::writeCSV() {
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			std::cout << *i << '\n';
		}
		return true;
	}

	bool HistoryLog::writeXML() {
		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			std::cout << *i << '\n';
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

	
	ImageHistoryLog::ImageHistoryLog() : LogDocument(std::make_shared<ImageHistorySchema>()) {
		// TODO Auto-generated constructor stub

	}

	ImageHistoryLog::~ImageHistoryLog() {
		// TODO Auto-generated destructor stub
	}

	bool ImageHistoryLog::writeHuman() {
		std::cout << "\n---------------------------------------------------\n";
		std::cout << "Image: " << m_title << '\n';
		std::cout << "Path : " << m_description << '\n';
		std::cout << "=====================================================\n";
		std::cout << "Date Time             version     Event      Comment\n\n";
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			//std::cout << *i << '\n';
			CSVArgs csvArgs(':');
			if (csvArgs.process(i->c_str()) == false) {
				return false;
			}

			std::cout << csvArgs.at(0) << "    ";
			std::cout << csvArgs.at(1) << "      ";
			//std::cout << csvArgs.at(2) << "  ";
			std::cout << csvArgs.at(4) << "  ";
			std::cout << csvArgs.at(3) << '\n';
			std::cout << "---------------------------------------------------\n";
		}
		return true;
	}
	
	bool ImageHistoryLog::writeJson() {
		return true;
	}

	bool ImageHistoryLog::writeCSV() {
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			std::cout << *i << '\n';
		}
		return true;
	}

	bool ImageHistoryLog::writeXML() {
		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			std::cout << *i << '\n';
			CSVArgs csvArgs(',');
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
	

} //namespace simplearchive