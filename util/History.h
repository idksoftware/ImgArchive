#pragma once

#include <memory>
#include <string>
#include <mutex>


namespace simplearchive {

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
	};

} // namespace simplearchive