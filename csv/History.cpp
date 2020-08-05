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
#include "ResultsPresentation.h"
#include "ResultsList.h"
#include "ImageHistoryResultsPresentation.h"
#include "SystemHistoryResultsPresentation.h"
#include "CLogger.h"

namespace simplearchive {

	std::unique_ptr<History> History::m_this = nullptr;
	std::once_flag History::m_onceFlag;
	std::string History::m_indexPath;
	std::string History::m_workspacePath;
	std::string History::m_systemHisteryPath;
	std::string History::m_changeLogPath;

	History::History() : 
		m_systemHistory(std::make_unique<SystemHistory>()),
		m_imageHistory(std::make_unique<ImageHistory>()),
		m_changeLog(std::make_unique<ChangeLog>())
	{

	}


	History::~History()
	{
	}

	bool History::init() {
		//if (m_systemHistory->init() == false) {
		//	return false;
		//}
		if (m_imageHistory->init() == false) {
			return false;
		}

		return true;
	}

	History& History::getHistory() {
		std::call_once(m_onceFlag,
			[] {
				if (m_this == nullptr) {
					m_this = std::make_unique<History>();
				}
			});
		return *(m_this);


	}

	void History::setPaths(const char* index, const char* workspace, const char* system) {
		m_indexPath = index;
		m_workspacePath = workspace;
		m_systemHisteryPath = system;
	}
#
	void History::initPaths() {
		m_systemHistory->setPath(m_systemHisteryPath.c_str());
		m_imageHistory->setPath(m_workspacePath.c_str(), m_indexPath.c_str());
	}

	bool History::newImage(const char* filepath, const char* comment) {

		initPaths();
		m_imageHistory->setPath(m_workspacePath.c_str(), m_indexPath.c_str());
		if (m_systemHistory->newImage(filepath, comment) == false) {
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



	bool History::checkinImage(const char* filepath, int version, const char* comment) {
		std::string versionString = Version2String(version);
		initPaths();
		if (m_systemHistory->add(filepath, version, comment, HistoryEvent::Event::CHECKIN) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (m_imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKIN) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}

	bool History::checkoutImage(const char* filepath, int version, const char* comment) {

		std::string versionString = Version2String(version);
		initPaths();
		if (m_systemHistory->add(filepath, version, comment, HistoryEvent::Event::CHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (m_imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::CHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;

	}

	bool History::uncheckoutImage(const char* filepath, int version, const char* comment) {
		std::string versionString = Version2String(version);
		initPaths();

		if (m_systemHistory->add(filepath, version, comment, HistoryEvent::Event::UNCHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (m_imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::UNCHECKOUT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}

	bool History::exportImage(const char* filepath, int version, const char* comment) {
		std::string versionString = Version2String(version);
		initPaths();

		if (m_systemHistory->add(filepath, version, comment, HistoryEvent::Event::EXPORT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (m_imageHistory->add(filepath, versionString.c_str(), comment, HistoryEvent::Event::EXPORT) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		return true;
	}

	bool History::systemHistoryLog(const char* scope) {
		CLogger& logger = CLogger::getLogger();
		m_systemHistory->setPath(m_systemHisteryPath.c_str());
	
		if (m_systemHistory->select(scope) == false) {
			logger.log(LOG_OK, CLogger::Level::ERR, "Cannot process archive history");
			return false;
		}
		std::shared_ptr<ResultsList> results = m_systemHistory->getResults();
		if (results == nullptr) {
			logger.log(LOG_OK, CLogger::Level::WARNING, "No results for archive history");
			return false;
		}
		SystemHistoryResultsPresentation resultsPresentation(*results);
		resultsPresentation.writeHuman();
	}

	bool History::imageHistoryLog(const char* scope)
	{
		CLogger& logger = CLogger::getLogger();
		m_imageHistory->setPath(m_workspacePath.c_str(), m_indexPath.c_str());
		
		if (m_imageHistory->select(scope) == false) {
			logger.log(LOG_OK, CLogger::Level::ERR, "Cannot process archive history");
			return false;
		}
		std::shared_ptr<ResultsList> results = m_imageHistory->getResults();
		if (results == nullptr) {
			logger.log(LOG_OK, CLogger::Level::WARNING, "No results for archive history");
			return false;
		}
		ImageHistoryResultsPresentation resultsPresentation(*results);
		resultsPresentation.writeHuman();
		return false;
	}

};