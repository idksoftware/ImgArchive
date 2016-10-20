#pragma once

#include "CheckDisk.h"

namespace simplearchive {
	class CheckDiskJournal;
	class ValidateReportingObject : public VisitingObject {
	protected:
		static std::string m_journalPath;
		std::string m_journalName;
		CheckDiskJournal *m_checkDiskJournal;
	public:
		ValidateReportingObject();
		~ValidateReportingObject();
		virtual bool process(const char *file, ReportStatus &status, const char *orginal = nullptr);
		bool save();
		static bool setPath(const char *path);
		const char *getJournalName();
		CheckDiskJournal &getJounrnal() { return *m_checkDiskJournal; };
	};

	class ValidateAndRepairingWorkspaceObject : public ValidateReportingObject {
		std::string m_archivePath;
		std::string m_workspacePath;
	public:
		ValidateAndRepairingWorkspaceObject(const char *archivePath, const char *workspacePath);
		virtual bool process(const char *file, ReportStatus &status, const char *orginal = nullptr);
	};

	class ValidateAndRepairingShadowObject : public ValidateReportingObject {
		std::string m_archivePath;
		std::string m_workspacePath;
	public:
		ValidateAndRepairingShadowObject(const char *archivePath, const char *workspacePath);
		virtual bool process(const char *file, ReportStatus &status, const char *orginal = nullptr);
	};

} // simplearchive
