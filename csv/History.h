#pragma once
#pragma once

#include <memory>
#include <string>
#include <mutex>
#include "LogDocument.h"
#include "DBDefines.h"
#include "MetaType.h"
#include "ResultsPresentation.h"

namespace simplearchive {

	class HistoryEvent;
	


	class SystemHistory;
	class ImageHistory;
	class ChangeLog;

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

		void initPaths();
	public:
		History();
		~History();

		History(History const&) = delete;
		void operator=(History const&) = delete;

		static void setPaths(const char* index, const char* workspace, const char* system);
		bool init();
		bool newImage(const char* filepath, const char* comment);
		bool checkinImage(const char* filepath, int version, const char* comment);
		bool checkoutImage(const char* filepath, int version, const char* comment);
		bool uncheckoutImage(const char* filepath, int version, const char* comment);
		bool exportImage(const char* filepath, int version, const char* comment);
		static History& getHistory();

		// This will write out the image histroy log
		bool systemHistoryLog(const char* addressScope, ResultsPresentation::FormatType formatType, const char* file);
		bool imageHistoryLog(const char* addressScope, ResultsPresentation::FormatType formatType, const char* file);
		//bool logImageHistory(const char *imagepath, LogDocument::FormatType formatType, const char* filepath);
		//bool logSystemHistory(const char* dateFrom, const char* dateTo, LogDocument::FormatType formatType, const char* filepath);
	};

} // namespace simplearchive


