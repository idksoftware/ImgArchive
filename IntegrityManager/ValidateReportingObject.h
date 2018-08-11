#pragma once

#include "CheckDisk.h"

namespace simplearchive {

	class CheckDiskJournal;
	class CDDaySummary;
	class CDYearSummary;
	class CheckDiskSummaryJounal;

	class ValidateReportingObject : public VisitingObject {
	protected:
		static std::string m_journalPath;
		std::string m_journalName;
		std::shared_ptr<CheckDiskJournal> m_checkDiskJournal;
		std::shared_ptr<CDDaySummary> m_checkDiskDaySummary;
		std::shared_ptr<CDYearSummary> m_checkDiskYearSummary;
		std::shared_ptr<CheckDiskSummaryJounal> m_checkDiskSummaryJounal;
		
	public:
		ValidateReportingObject() : m_checkDiskJournal(std::make_shared<CheckDiskJournal>()),
									m_checkDiskSummaryJounal(std::make_shared<CheckDiskSummaryJounal>()) {}
		virtual ~ValidateReportingObject() = default;
		virtual bool process(const char *file, ReportStatus &status, const char *orginal = nullptr);
		bool save();
		static bool setPath(const char *path);
		const char *getJournalName();
		CheckDiskJournal &getJournal() { return *m_checkDiskJournal; };
		CDYearSummary &getYearSummary() { return *m_checkDiskYearSummary; };
		CDDaySummary &getDaySummary() { return *m_checkDiskDaySummary; };

		virtual void startYear(const char *year);
		virtual void endYear(const char *year);
		virtual void startDay(const char *day);
		virtual void endDay(const char *day);
		CheckDiskSummaryJounal& GetCheckDiskSummaryJounal() { return *m_checkDiskSummaryJounal; }
	};

	class ValidateAndRepairingWorkspaceObject : public ValidateReportingObject {
		std::string m_archivePath;
		std::string m_workspacePath;
	public:
		ValidateAndRepairingWorkspaceObject(const char *archivePath, const char *workspacePath);
		virtual bool process(const char *file, ReportStatus &status, const char *orginal = nullptr);
	};

	class ValidateAndRepairingMasterObject : public ValidateReportingObject {
		std::string m_archivePath;
		std::string m_workspacePath;
	public:
		ValidateAndRepairingMasterObject(const char *archivePath, const char *workspacePath);
		virtual bool process(const char *file, ReportStatus &status, const char *orginal = nullptr);
	};

} // simplearchive
