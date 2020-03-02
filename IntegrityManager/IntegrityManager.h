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

namespace simplearchive {

class IMCompletedSummary {
	std::string m_summary;
	std::string m_result;
public:
	IMCompletedSummary() = default;
	virtual ~IMCompletedSummary() = default;
	void setSummary(const char *s) { m_summary = s; };
	const char *getSummary() { return m_summary.c_str(); };
	void setResult(const char *s) { m_result = s; };
	const char *getResult() { return m_result.c_str(); };
};



class IntegrityManager {
public:
	enum class Scope {
		Workspace,		//* Initalise an archive with the default 
		Master,			//* Show
		Derivative,			//* Show
		All,			//* Show
		Main			//* show version
	};
	enum class VerifyBackups {
		Backup_1,		//* Initalise an archive with the default 
		Backup_2,			//* Show
		Both,		//* Show
		None
	};
private:
	std::string m_archivePath;
	std::string m_derivativePath;
	std::string m_workspacePath;
	std::string m_homePath;
	
	std::string m_masterBackupPath1;
	std::string m_masterBackupPath2;

	bool m_masterBackup1enabled;
	bool m_masterBackup2enabled;

	std::string m_derivativeBackupPath1;
	std::string m_derivativeBackupPath2;

	bool m_derivativeBackup1enabled;
	bool m_derivativeBackup2enabled;

	IntegrityManager()
		: m_masterBackup1enabled(false),
		m_masterBackup2enabled(false),
		m_derivativeBackup1enabled(false),
		m_derivativeBackup2enabled(false)
	{};

public:
	IntegrityManager(IntegrityManager const&) = delete;
	void operator=(IntegrityManager const&) = delete;
	IntegrityManager(IntegrityManager&&) = delete;                  // Move construct
	IntegrityManager& operator=(IntegrityManager &&) = delete;      // Move assign

	bool addDayFolder(const char *rootName, const char *folderName);
	bool addFile(const char *rootName, const char *folderPath, const char *fileName);

	virtual ~IntegrityManager() = default;
	bool addMasterDayFolder(const char *folderName);
	bool addDerivativeDayFolder(const char *folderName);
	bool addDerivativeFile(const char *folderPath, const char *fileName);
	bool addMasterFile(const char *folderPath, const char *fileName);
	
	bool makeList();
	bool validate(IMCompletedSummary& imCompletedSummary, Scope scope, bool main, VerifyBackups verifyBackups, bool repair);
	//bool repair(IMCompletedSummary& imCompletedSummary, bool workspace, bool Master);
	
	static IntegrityManager &get();
	void setPaths(const char* archivePath, const char *derivativePath, const char* workspacePath, const char* homePath);
	void setMasterBackupPaths(const char* backupPath1, const char* backupPath2, bool backup1enabled, bool backup2enabled);
	void setDerivativeBackupPaths(const char* backupPath1, const char* backupPath2, bool backup1enabled, bool backup2enabled);
};

} /* namespace simplearchive */

