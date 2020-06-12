#include <fstream>
#include <sstream>
#include "LogName.h"
#include "ImportJournal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	

	ImportJournalItem::ImportJournalItem(const char *filepath) {
		m_filepath = filepath;
		m_result = ImportJournal::Result::Incomplete;
		m_validated = false;
	}

	void ImportJournalItem::setResult(ImportJournal::Result result) {
		m_result = result;
	}

	void ImportJournalItem::setLocation(const char *location) {
		m_location = location;
	}

	void ImportJournalItem::setValidated()
	{
		m_validated = true;
	}

	const char *ImportJournalItem::ResultString(ImportJournal::Result& result) {
		switch (result) {
		case ImportJournal::Result::Incomplete:
			return "Incomplete";
		case ImportJournal::Result::Imported:
			return "Imported";
		case ImportJournal::Result::Duplicate:
			return "Duplicate";
		case ImportJournal::Result::Error:
			return "Error";
		case ImportJournal::Result::Unknown:
		default:
			return "Unknown";
		}
	}

	ImportJournal::Result ImportJournalItem::getResultEnum() {
		return m_result;
	}

	const char *ImportJournalItem::getResult() {
		return ResultString(m_result);
	}

	const char *ImportJournalItem::getSourceImage() {
		return m_filepath.c_str();
	}

	const char *ImportJournalItem::getLocation() {
		return m_location.c_str();
	}

	bool ImportJournalItem::isValidated() {
		return m_validated;
	}

	std::string ImportJournalItem::toString() {
		std::string tmp;
		tmp += m_filepath;
		tmp += ',';
		tmp += ResultString(m_result);
		tmp += ',';
		tmp += m_location;
		tmp += ',';
		tmp += m_validated ? "True" : "False";
		return tmp;
	}

	

	ImportJournal::ImportJournal(std::string &name)
	{
		m_count = 0;	
		m_logNameStr = name;
		int lenNoExt = m_logNameStr.find_last_of(".");
		m_xmlNameStr = m_logNameStr.substr(0, lenNoExt);
		m_xmlNameStr += ".xml";
	}


	ImportJournal::~ImportJournal()
	{
		
	}

	bool ImportJournal::add(const char *filepath) {

		m_index.insert(std::make_pair(filepath, m_count));
		m_count++;
		push_back(std::make_shared<ImportJournalItem>(filepath));
		return true;
	}

	bool ImportJournal::update(const char *filepath, Result result, const char *location) {
		if (m_index.find(filepath) == m_index.end()) {
			return false;
		}

		int& pos = m_index.at(filepath);
		std::shared_ptr<ImportJournalItem> item = at(pos);
		item->setResult(result);
		if (location != 0) {
			item->setLocation(location);
		}
		return true;
	}

	bool ImportJournal::setCurrent(const char *imagesPath) {
		int& pos = m_index.at(imagesPath);
		m_current = pos;
		return true;
	}
	size_t ImportJournal::getCurrentImageIndex() {
		return m_current;
	}

	bool ImportJournal::write(const char *journalFilePath) {
		
		std::ofstream file(journalFilePath);
		if (file.is_open() == false) {
			return false;
		}
		
		for (auto i = begin(); i != end(); i++) {
			std::shared_ptr<ImportJournalItem> item = *i;
			std::string line = item->toString();
			//printf("%s\n", line.c_str());
			file << line << '\n';

		}
		
		file.close();
		
		return true;
	}

	bool ImportJournal::writeXML(const char *journalFilePath) {

		std::ofstream file(journalFilePath);
		if (file.is_open() == false) {
			return false;
		}
		ExifDateTime timeCompleted;
		timeCompleted.now();
		timeCompleted.toString();
		file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<ImportJournal Completed=\"" << timeCompleted.toString() << "\">\n";

		for (auto i = begin(); i != end(); i++) {
			std::shared_ptr<ImportJournalItem> item = *i;
			file << "\t<Event>\n";

			file << writeTag("SourceImage", item->getSourceImage(), 2);
			file << writeTag("Result", item->getResult(), 2);
			file << writeTag("Location", item->getLocation(), 2);
			file << writeTag("Validated", (item->isValidated())?"True":"False", 2);
			file << "\t</Event>\n";
		}
		file << "</ImportJournal>\n";
		return true;
	}

	std::string ImportJournal::writeTag(const char *tag, const std::string& value, int tab) {
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

	std::string ImportJournal::writeTag(const char *tag, const int value, int tab) {
		std::ostringstream xml;
		for (int i = 0; i < tab; i++) {
			xml << '\t';
		}
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
		return xml.str();
	}

	std::string ImportJournalManager::m_journalFilePath;
	
	ImportJournalManager::ImportJournalManager() {
		LogName logName;
		std::string logNameStr = logName.makeName(m_journalFilePath.c_str(), "ij", "log", LogName::ALWAYS_CREATE);
		m_importJournal = std::make_unique<ImportJournal>(logNameStr);

	};

	ImportJournal& ImportJournalManager::GetJournal()
	{
		static ImportJournalManager INSTANCE;
		return *(INSTANCE.m_importJournal);
	}

	bool ImportJournalManager::save() {
		//LogName logName;
		ImportJournal& ip = GetJournal();
		///std::string logNameStr = logName.makeName(m_journalFilePath.c_str(), "ij", "log", LogName::ALWAYS_CREATE);
		ip.write(); //logNameStr.c_str());
		
		//logNameStr = logName.makeName(m_journalFilePath.c_str(), "ij", "xml", LogName::ALWAYS_CREATE);
		ip.writeXML();

		return true;
	}

	ImportJournalManager::~ImportJournalManager() {
		
	}

} /* namespace simplearchive */
