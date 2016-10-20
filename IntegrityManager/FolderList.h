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

#ifndef FOLDERLIST_H_
#define FOLDERLIST_H_

#include <string>
#include "ArchiveProcessor.h"

namespace simplearchive {

	class ShowCheckedOut : public ShadowFolderVistor {
	public:
		ShowCheckedOut(const char *archivePath) : ShadowFolderVistor(archivePath) {};
		virtual bool doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject);
	};

	class ShowUncheckedOutChanges : public WorkspaceFolderVistor {
	public:
		ShowUncheckedOutChanges(const char *archivePath, const char *workspacePath) : WorkspaceFolderVistor(archivePath, workspacePath) {
			m_visitingObject = new ValidateReportingObject;
		};
		virtual bool doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject);
		virtual ~ShowUncheckedOutChanges() { delete m_visitingObject; }
	};


	class FolderFile;
	/**
	 * @Brief This class manages the image folder list in the archive.
	 * It records changes to the image folder and maintains an XML list
	 * of folders.
	 */
	class FolderList {
	public:
		typedef enum {
			READING_SHADOW,
			READING_WORKSPACE,
			READING_BOTH,
			UNKNOWN
		} Action;
	private:
		std::string m_archivePath;
		std::string m_workspacePath;
		std::string m_workspaceJournalName;
		std::string m_shadowJournalName;
		bool makeXML();

		bool validateAndRepairShadow();
		bool validateOnlyShadow();

		bool validateAndRepairWorkspace();
		bool validateOnlyWorkspace();

		bool validateShadow(ValidateReportingObject &validateReportingObject);
		bool validateWorkspace(ValidateReportingObject &validateReportingObject);

		bool fixWorkspace(const char *jouralFile);
		Action m_action;
	public:
		FolderList(const char *archivePath);
		FolderList(const char *archivePath, const char *workspacePath);
		virtual ~FolderList();

		bool addDayFolder(const char *folderName);
		bool incFolders(const char *folderName);
		bool incFiles(const char *folderName);
		bool makeList();
		bool validate();
		bool validateAndRepair();
		bool fix();
		void SetAction(Action action) {
			m_action = action;
		}

		bool showCheckedOut(const char *addressScope);
		bool showUncheckedOutChanges(const char *addressScope);
	};

} /* namespace simplearchive */
#endif /* FOLDERLIST_H_ */
