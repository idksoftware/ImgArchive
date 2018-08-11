#pragma once
#include "ValidateReportingObject.h"
#include "AddressScope.h"

namespace simplearchive {

	class VisitingObject;

	class ArchiveFolderVistor {
	protected:
		std::string m_archivePath;
		std::string m_workspacePath;
		std::shared_ptr<VisitingObject> m_visitingObject;
		AddressScope m_AddressScope;
		virtual VisitingObject& getVisitingObject() = 0;
	public:
		ArchiveFolderVistor(const char *archivePath);
		ArchiveFolderVistor(const char *archivePath, const char *workspacePath);
		virtual ~ArchiveFolderVistor() {};
		virtual bool process(const char *addressScope) = 0;
		virtual bool doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject) = 0;
	};

	class MasterFolderVistor : public ArchiveFolderVistor {

	protected:
		virtual VisitingObject& getVisitingObject() { return *m_visitingObject; };
	public:
		MasterFolderVistor(const char *archivePath) : ArchiveFolderVistor(archivePath) {};
		MasterFolderVistor(const char *archivePath, const char *workspacePath)
			: ArchiveFolderVistor(archivePath, workspacePath) {};
		virtual ~MasterFolderVistor() {};
		bool process(const char *addressScope);
		virtual bool doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject) = 0;
	};

	class WorkspaceFolderVistor : public ArchiveFolderVistor {

	protected:
		virtual VisitingObject& getVisitingObject() { return *m_visitingObject; };
	public:
		WorkspaceFolderVistor(const char *archivePath) : ArchiveFolderVistor(archivePath) {};
		WorkspaceFolderVistor(const char *archivePath, const char *workspacePath)
			: ArchiveFolderVistor(archivePath, workspacePath) {};
		virtual ~WorkspaceFolderVistor() {};
		bool process(const char *addressScope);
		virtual bool doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject) = 0;
	};

	class ValidateReportingObject;
	class ValidateWorkspace : public WorkspaceFolderVistor {
		std::shared_ptr<ValidateReportingObject> m_validateReportingObject;
	public:
		ValidateWorkspace(const char *archivePath, const char *workspacePath) : WorkspaceFolderVistor(archivePath, workspacePath),
			m_validateReportingObject(std::shared_ptr<ValidateReportingObject>()) {};
			
		virtual bool doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject);
	};

};
