#pragma once

#include <memory>
#include <string>
#include <mutex>
#include "LogDocument.h"
#include "DBDefines.h"
#include "MetaType.h"

namespace simplearchive {

	class HistoryEvent;

	class ImageHistorySchema : public MTTableSchema {
	public:
		ImageHistorySchema() : MTTableSchema(TABLE_IMAGE_HISTORY) {
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Integer, DB_VERSION));// template
			add(MTSchema(MTSchema::Text, DB_EVENT));
			add(MTSchema(MTSchema::Text, DB_COMMENT));// template
		}
	};

	class SystemHistorySchema : public MTTableSchema {
	public:
		SystemHistorySchema() : MTTableSchema(TABLE_SYSTEM_HISTORY) {
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));	
			add(MTSchema(MTSchema::Integer, DB_VERSION));// template
			add(MTSchema(MTSchema::Text, DB_EVENT));
			add(MTSchema(MTSchema::Text, DB_COMMENT));// template
		}
	};
	
	class ImageHistoryRow : public MTRow {
		static ImageHistorySchema m_tableSchema;
	public:
		ImageHistoryRow();
		ImageHistoryRow(const char *filepath, const char *version, const char *comment, const HistoryEvent &he);
		ImageHistoryRow(const MTRow &row) : MTRow(m_tableSchema) {

			for (unsigned int i = 0; i < row.size(); i++) {
				MTColumn& c1 = *at(i);
				MTColumn& c2 = *row.at(i);
				c1 = c2;
			}
		}
	};

	class SystemHistoryRow : public MTRow {
		static SystemHistorySchema m_tableSchema;
	public:
		SystemHistoryRow();
		SystemHistoryRow(const char *filepath, const char *version, const char *comment, const HistoryEvent &he);
		SystemHistoryRow(const MTRow &row) : MTRow(m_tableSchema) {

			for (unsigned int i = 0; i < row.size(); i++) {
				MTColumn& c1 = *at(i);
				MTColumn& c2 = *row.at(i);
				c1 = c2;
			}
		}
	};


	class SystemHistory;
	class ImageHistory;
	class ChangeLog;

	class SystemHistoryLog : public LogDocument {

	public:
		SystemHistoryLog();
		virtual ~SystemHistoryLog();
		bool writeHuman();
		bool writeXML();
		bool writeCSV();
		bool writeJson();
		bool writeHtml();
	};

	class ImageHistoryLog : public LogDocument {
		
	public:
		ImageHistoryLog();
		virtual ~ImageHistoryLog();
		bool writeHuman();
		bool writeXML();
		bool writeCSV();
		bool writeJson();
		bool writeHtml();
		
	};

	class History
	{
		static std::unique_ptr<History> m_this;
		static std::once_flag m_onceFlag;


		static std::string m_indexPath;
		static std::string m_workspacePath;
		static std::string m_systemHisteryPath;
		static std::string m_changeLogPath;
		std::unique_ptr<SystemHistory> m_systemHistory;
		std::unique_ptr<ImageHistory> m_imageHistory;
		std::unique_ptr<ChangeLog> m_changeLog;
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
		bool logImageHistory(const char *imagepath, LogDocument::FormatType formatType, const char* filepath);
		bool logSystemHistory(const char* dateFrom, const char* dateTo, LogDocument::FormatType formatType, const char* filepath);
	};

} // namespace simplearchive
