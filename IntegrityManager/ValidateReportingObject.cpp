#include "ValidateReportingObject.h"
#include "CheckDiskJournal.h"
#include "LogName.h"

namespace simplearchive {

	std::string ValidateReportingObject::m_journalPath;

	

	bool ValidateReportingObject::setPath(const char *path) {
		m_journalPath = path;
		return true;
	}

	bool ValidateReportingObject::process(const char *file, ReportStatus &status, const char *orginal) {
		CDDaySummary& daySummary = getDaySummary();
		daySummary.setStatus(status);

		if (orginal == nullptr) {
//			printf("File: %s Status: %s\n", file, status.toString());
			m_checkDiskJournal->add(file, status);
		}
		else {
//			printf("File: %s Status: %s: file: %s\n", file, status.toString(), orginal);
			m_checkDiskJournal->add(file, status, orginal);
		}
		return true;
	}

	const char *ValidateReportingObject::getJournalName() {
		return m_journalName.c_str();
	}

	bool ValidateReportingObject::save() {
		LogName logName;

		std::string logNameStr = logName.makeName(m_journalPath.c_str(), "cdj", "log", LogName::ALWAYS_CREATE);
		m_checkDiskJournal->write(logNameStr.c_str());
		m_journalName = logNameStr;
		logNameStr = logName.makeName(m_journalPath.c_str(), "cdj", "xml", LogName::ALWAYS_CREATE);
		m_checkDiskJournal->writeXML(logNameStr.c_str());

		logNameStr = logName.makeName(m_journalPath.c_str(), "cdsj", "log", LogName::ALWAYS_CREATE);
		m_checkDiskSummaryJounal->write(logNameStr.c_str());
		m_journalName = logNameStr;
		logNameStr = logName.makeName(m_journalPath.c_str(), "cdsj", "xml", LogName::ALWAYS_CREATE);
		m_checkDiskSummaryJounal->writeXML(logNameStr.c_str());

		return true;
	}

	void ValidateReportingObject::startYear(const char *year) {
//		printf("Visiting start Year %s", year);
		m_checkDiskYearSummary = std::make_shared<CDYearSummary>(year);
		
	};
	void ValidateReportingObject::endYear(const char *year) {
//		printf("Visiting end Year %s", year);
		m_checkDiskSummaryJounal->add(m_checkDiskYearSummary);
	};
	void ValidateReportingObject::startDay(const char *day) {
//		printf("Visiting start Day %s", day);
		m_checkDiskDaySummary = std::make_shared<CDDaySummary>(day);
		
		
	};
	void ValidateReportingObject::endDay(const char *day) {
//		printf("Visiting end Day %s", day);		
		CDYearSummary& yearSummary = getYearSummary();
		yearSummary.add(m_checkDiskDaySummary);
	};



	ValidateAndRepairingWorkspaceObject::ValidateAndRepairingWorkspaceObject(const char *archivePath, const char *workspacePath) :
										m_archivePath(archivePath), m_workspacePath(workspacePath)
	{}
	
	bool ValidateAndRepairingWorkspaceObject::process(const char *file, ReportStatus &status, const char *orginal) {
		if (status.get() == ReportStatus::Unchanged) {
			return true;
		}
		if (orginal == nullptr) {
//			printf("File: %s Status: %s\n", file, status.toString());
			m_checkDiskJournal->add(file, status);
		}
		else {
//			printf("File: %s Status: %s: file: %s\n", file, status.toString(), orginal);
			m_checkDiskJournal->add(file, status, orginal);
		}

		switch (status.get()) {
		case ReportStatus::ContentChanged:
			{
				std::string fileStr = file;
				std::string year = fileStr.substr(0, 4);
				std::string dmy = fileStr.substr(0, 10);
				std::string name = fileStr.substr(11, fileStr.length() - 9);
				std::string archivePath = m_archivePath;
				archivePath += '/'; archivePath += year;
				archivePath += '/'; archivePath += dmy;
				archivePath += "/data/"; archivePath += name;
				
				if (SAUtils::FileExists(archivePath.c_str()) == false) {
					return false;
				}
				std::string workspacePath = m_workspacePath;
				workspacePath += '/'; workspacePath += year; workspacePath += '/';
				workspacePath += file;
				if (SAUtils::FileExists(workspacePath.c_str()) == false) {
					return false;
				}
				if (SAUtils::copy(archivePath.c_str(), workspacePath.c_str()) == false) {
					return false;
				}
			}
			return true;
		case ReportStatus::NameChanged:
			{
				std::string fileStr = file;
				std::string year = fileStr.substr(0, 4);
				//std::string dmy = fileStr.substr(0, 10);
				//std::string name = fileStr.substr(11, fileStr.length() - 9);
				std::string workspacePath = m_workspacePath;
				workspacePath += '/'; workspacePath += year; workspacePath += '/';
				workspacePath += file;
				if (SAUtils::FileExists(workspacePath.c_str()) == false) {
					return false;
				}
				// Delete the file as the orginal will be copied from the Master folder.
				if (SAUtils::delFile(workspacePath.c_str()) == false) {
					return false;
				}
				/*
				std::string fullWorkspacePath = m_workspacePath;
				fullWorkspacePath += '/'; fullWorkspacePath += year;
				fullWorkspacePath += '/'; fullWorkspacePath += dmy;
				fullWorkspacePath += '/';
				std::string orginalPath = fullWorkspacePath;
				orginalPath += orginal;
				
				if (SAUtils::FileExists(orginalPath.c_str()) == true) {
					// An image is using the orginal image name so rename to <name>_new.<ext>
					
					std::string newName;
					std::string orginalStr = orginal;
					int pos = orginalStr.find_last_of('.');
					if (pos == std::string::npos) {
						// no ext
						newName = orginalStr;
						newName += "_new";
					}
					else {
						
						newName = orginalStr.substr(pos, orginalStr.length() - pos);
						newName += "_new";
						newName += orginalStr.substr(pos, orginalStr.length() - pos);
					}
					std::string newPath = fullWorkspacePath;
					newPath += newName;
					if (SAUtils::rename(orginalPath.c_str(), newPath.c_str()) == false) {
						return false;
					}
				}
					
				*/
			}
			return true;
		case ReportStatus::Missing:
			{
				std::string fileStr = file;
				std::string year = fileStr.substr(0, 4);
				std::string dmy = fileStr.substr(0, 10);
				std::string name = fileStr.substr(11, fileStr.length() - 9);
				std::string archivePath = m_archivePath;
				archivePath += '/'; archivePath += year;
				archivePath += '/'; archivePath += dmy;
				archivePath += "/data/"; archivePath += name;

				if (SAUtils::FileExists(archivePath.c_str()) == false) {
					// throw
				}
				std::string workspacePath = m_workspacePath;
				workspacePath += '/'; workspacePath += year; workspacePath += '/';
				workspacePath += file;
				
				if (SAUtils::copy(archivePath.c_str(), workspacePath.c_str()) == false) {
					// throw
				}
			}
			return true;
		case ReportStatus::Added:
			return true;
		case ReportStatus::CheckedOutNoChange:
			return true;
		case ReportStatus::CheckedOutChanged:
			return true;
		case ReportStatus::Unchanged:
			return true;
		case ReportStatus::Unknown:
			return true;
		default:
			return true;
		}
		return true;
	}

	ValidateAndRepairingMasterObject::ValidateAndRepairingMasterObject(const char *archivePath, const char *workspacePath)
						: m_archivePath(archivePath), m_workspacePath(workspacePath) {}

	bool ValidateAndRepairingMasterObject::process(const char *file, ReportStatus &status, const char *orginal) {
		
		if (orginal == nullptr) {
//			printf("File: %s Status: %s\n", file, status.toString());
			m_checkDiskJournal->add(file, status);
		}
		else {
			printf("File: %s Status: %s: file: %s\n", file, status.toString(), orginal);
			m_checkDiskJournal->add(file, status, orginal);
		}
		return true;
	}


} // simplearchive