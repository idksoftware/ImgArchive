#pragma once

#include <memory>
#include <string>
#include <mutex>
#include "LogDocument.h"
#include "DBDefines.h"
#include "MetaType.h"

namespace simplearchive {

	class ImageHistorySchema : public MTTableSchema {
	public:
		ImageHistorySchema() : MTTableSchema("ImageHistory") {
			add(MTSchema(MTSchema::Text, DB_COMMENT));// template
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
			add(MTSchema(MTSchema::Text, DB_EVENT));
			add(MTSchema(MTSchema::Integer, DB_VERSION));// template
		}
	};



	class SystemHistory;
	class ImageHistory;
	class ChangeLog;

	class HistoryLog : public LogDocument {

	public:
		HistoryLog();
		virtual ~HistoryLog();
		bool writeHuman();
		bool writeXML();
		bool writeCSV();
		bool writeJson() ;
	};

	class ImageHistoryLog : public LogDocument {

	public:
		ImageHistoryLog();
		virtual ~ImageHistoryLog();
		bool writeHuman();
		bool writeXML();
		bool writeCSV();
		bool writeJson();
	};

	class History
	{
		static std::unique_ptr<History> m_this;
		static std::once_flag m_onceFlag;


		static std::string m_indexPath;
		static std::string m_workspacePath;
		static std::string m_systemHisteryPath;
		static std::string History::m_changeLogPath;
		std::unique_ptr<SystemHistory> systemHistory;
		std::unique_ptr<ImageHistory> imageHistory;
		std::unique_ptr<ChangeLog> changeLog;
	public:
		History();
		~History();

		History(History const&) = delete;
		void operator=(History const&) = delete;

		static void setPaths(const char *index, const char *workspace, const char *system);
		bool init();
		bool newImage(const char *filepath, const char *comment);
		bool checkinImage(const char *filepath, int version, const char *comment);
		bool checkoutImage(const char *filepath, int version, const char *comment);
		bool uncheckoutImage(const char *filepath, int version, const char *comment);
		bool exportImage(const char *filepath, int version, const char *comment);
		static History &getHistory();

		// This will write out the image histroy log
		bool logImageHistory(const char *filepath, LogDocument::FormatType formatType);
	};

} // namespace simplearchive