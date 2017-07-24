#include "History.h"

#include <memory>
#include <sstream>
#include <iomanip>

#include "HistoryEvent.h"
#include "ImageHistory.h"
#include "SystemHistory.h"
#include "ChangeLog.h"
#include "ErrorCode.h"

namespace simplearchive {

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

		/*
		std::call_once(m_onceFlag,
		[] {
		m_this.reset(new ChangeLog);
		LogFilename logFilename(m_logpath.c_str());
		m_filename = logFilename.filepath();
		m_logfile.open(m_filename.c_str(), ios::out | ios::app);
		});

		return *m_this.get();
		*/
	}

	void History::setPaths(const char *index, const char *workspace, const char *system) {
		m_indexPath = index;
		m_workspacePath = workspace;
		m_systemHisteryPath = system;
		SystemHistory::setPath(system, index);
		ImageHistory::setPath(workspace, index);
	}

	bool History::newImage(const char *filepath, const char *comment) {
		
		if (systemHistory->add(filepath, "0000", comment, HistoryEvent::ADDED) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}
	
		if (imageHistory->add(filepath, "0000", comment, HistoryEvent::ADDED) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
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

		if (systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::CHECKIN) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}

		if (imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::CHECKIN) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}

	bool History::checkoutImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::CHECKOUT) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}

		if (imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::CHECKOUT) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
		
	}

	bool History::uncheckoutImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::UNCHECKOUT) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}

		if (imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::UNCHECKOUT) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}

	bool History::exportImage(const char *filepath, int version, const char *comment) {
		std::string versionString = Version2String(version);
		if (systemHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::EXPORT) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}

		if (imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::EXPORT) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}

} //namespace simplearchive