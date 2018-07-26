#pragma once
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include "SAUtils.h"
#include "LogDocument.h"
#include "ExifDateTime.h"

namespace simplearchive {

	class ImportJournalItem;
	//*
	class ImportJournal : protected std::vector <std::shared_ptr<ImportJournalItem>>
	{
		std::string writeTag(const char *tag, const std::string& value, int tab);
		std::string writeTag(const char *tag, const int value, int tab);
		std::map<std::string, int> m_index;
		std::string m_logNameStr;
		std::string m_xmlNameStr;
		int m_count;
		int m_current;
	public:
		typedef enum {
			Incomplete,
			Imported,
			Duplicate,
			Error,
			Unknown
		} Result;
	private:
		bool write(const char *journalFilePath);
		bool writeXML(const char *journalFilePath);
	public:
		ImportJournal(std::string &logName);
		virtual ~ImportJournal();
		bool add(const char *filepath);
		bool update(const char *filepath, Result result, const char *location = 0);
		
		int numberOfImages() { return size(); }
		bool setCurrent(const char *imagesPath);
		int getCurrentImageIndex();
		bool write() {
			return write(m_logNameStr.c_str());
		}
		bool writeXML() {
			return writeXML(m_xmlNameStr.c_str());
		}
	};

	class ImportJournalManager {
		static std::string m_journalFilePath;
		static std::unique_ptr<ImportJournalManager> m_instance;
		static std::once_flag m_onceFlag;
		ImportJournalManager();
		ImportJournalManager(const ImportJournalManager& src);
		ImportJournalManager& operator=(const ImportJournalManager& rhs);
		std::unique_ptr<ImportJournal> m_importJournal;
	public:
		static void setJournalFilePath(const char *journalRoot) {
			m_journalFilePath = journalRoot;
			if (SAUtils::DirExists(m_journalFilePath.c_str()) == false) {
				SAUtils::mkDir(m_journalFilePath.c_str());
			}
			m_journalFilePath += "/import";
			if (SAUtils::DirExists(m_journalFilePath.c_str()) == false) {
				SAUtils::mkDir(m_journalFilePath.c_str());
			}

		}
		static ImportJournal& GetJournal();
		static bool save();
		virtual ~ImportJournalManager();
	};
} /* namespace simplearchive */

