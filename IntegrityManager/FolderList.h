/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#pragma once

#include <string>
#include "ArchiveProcessor.h"

namespace simplearchive {

	class ShowCheckedOut : public MasterFolderVistor {
	public:
		explicit ShowCheckedOut(const char *archivePath) : MasterFolderVistor(archivePath) {};
		virtual bool doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject);
	};

	class ShowUncheckedOutChanges : public WorkspaceFolderVistor {
	public:
		ShowUncheckedOutChanges(const char *archivePath, const char *workspacePath) : WorkspaceFolderVistor(archivePath, workspacePath) {
			m_visitingObject = std::make_shared<ValidateReportingObject>();
		};
		virtual bool doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject);
		virtual ~ShowUncheckedOutChanges() = default;
	};


	class FolderFile;
	class IMCompletedSummary;

	/**
	 * @Brief This class manages the image folder list in the archive.
	 * It records changes to the image folder and maintains an XML list
	 * of folders.
	 */
	class FolderList {
	public:
		enum class Action {
			READING_MASTER,
			READING_WORKSPACE,
			READING_DERIVATIVE,
			READING_ALL,
			READING_MAIN
		};
		enum class VerifyBackups {
			Backup_1,		//* Initalise an archive with the default 
			Backup_2,			//* Show
			Both,		//* Show
			None
		};
	private:
		//std::string m_archivePath;
		std::string m_workspacePath;
		std::string m_workspaceJournalName;
		std::string m_MasterJournalName;
		
		bool makeXML(const char* archivePath);

		bool validateAndRepairMaster(const char* archivePath, const char* workspacePath, IMCompletedSummary& imCompletedSummar, VerifyBackups verifyBackups);
		bool validateOnlyMaster(const char* archivePath, IMCompletedSummary& imCompletedSummar, VerifyBackups verifyBackups);

		

		bool validateMaster(const char* archivePath, ValidateReportingObject &validateReportingObject, VerifyBackups verifyBackups);
		bool validateDatabase(const char* archivePath, ValidateReportingObject& validateReportingObject, VerifyBackups verifyBackups);

		bool validateAndRepairWorkspace(const char* archivePath, IMCompletedSummary& imCompletedSummar, VerifyBackups verifyBackups);
		bool validateOnlyWorkspace(const char* archivePath, IMCompletedSummary& imCompletedSummar, VerifyBackups verifyBackups);
		bool validateWorkspace(const char* archivePath, ValidateReportingObject &validateReportingObject, VerifyBackups verifyBackups);

		bool fixWorkspace(const char* archivePath, const char *jouralFile);
		Action m_action;

		std::string makeDBPathCSV(const char *archivePath) const;
		std::string makeDBPathXML(const char* archivePath) const;
	public:
		explicit FolderList();
		FolderList(const char *workspacePath);
		virtual ~FolderList();

		bool addDayFolder(const char* archivePath, const char *folderName);
		bool incFolders(const char* archivePath, const char *folderName = "fdata");
		bool incFiles(const char* archivePath, const char *folderName);
		bool makeList(const char* archivePath);
		
		bool fix(const char* archivePath);
		void SetAction(Action action) {
			m_action = action;
		}

		bool showCheckedOut(const char* archivePath, const char *addressScope);
		bool showUncheckedOutChanges(const char* archivePath, const char* workspacePath, const char *addressScope);

		bool validate(const char *archivePath, IMCompletedSummary& imCompletedSummary, VerifyBackups verifyBackups, bool repair);
		//bool validateAndRepair(IMCompletedSummary& imCompletedSummary);
	};

} /* namespace simplearchive */

