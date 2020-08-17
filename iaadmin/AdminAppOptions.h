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

#ifndef ADMINAPPOPTIONS_H_
#define ADMINAPPOPTIONS_H_
#include <string>

namespace simplearchive {

class AppOptions {
public:
	enum class CommandMode {
		CM_InitArchive, //* Initalise an archive with the default prameter
		CM_Show,        //* Show
		CM_About,       //* show version
		CM_Sync,		//* Sync archive to backup(s)
		CM_Validate,    //* Validate the archive.
		CM_Purge,		//* Purge deleted images
		CM_Test,        //* Test the archive
		CM_Backup,		//* Backup archive to exteral media
		CM_Config,		//* Configure the system
		CM_Allow,		//* Set which file extension are allow into the archive.
		CM_Unknown
	};

	enum class VerifyOperation {
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

	enum class ShowOperation {
		Settup,
		CheckedOut,		//* Show the images that are checked out 
		ModifiedNotCheckedOut,			//* Show
		
	};


private:
	static AppOptions *m_this;
	static CommandMode m_commandMode;
	static std::string m_name;
	static std::string m_comment;
	static std::string m_imageAddress;
	static std::string m_homePath;
	static std::string m_workspacePath;
	static std::string m_masterPath;
	static std::string m_configPath;
	static std::string m_derivativePath;
	static std::string m_repositoryPath;
	static std::string m_catalogPath;
	static std::string m_picturePath;
	static std::string m_wwwImagePath;
	static std::string m_configOption;
	static std::string m_configValue;
	static std::string m_optionBlock;
	static VerifyOperation m_verifyOperation;
	static VerifyBackups m_verifyBackups;
	static ShowOperation m_showOperation;
	static std::string m_outputFile;
	static std::string m_textOutputType;
	static bool m_repair;
	static bool m_validateMain;
	static bool m_users;
	static bool m_setHomeEnv;
	static std::string m_mediaSize;
	static std::string m_mediaPath;
	static std::string m_fromDate;
	static std::string m_toDate;

	bool m_error;
	/**
		@brief Set to true if no archive configuration found. 
	*/
	friend class AdminArgvParser;

	bool m_configured;
	void setCommandMode(const AppOptions::CommandMode mode);
	bool setCommandMode(const char *modeString);
	void setName(const char *name);
	void setHomePath(const char *homePath);
	void setWorkspacePath(const char *workspacePath);
	void setMasterPath(const char *masterPath);
	void setConfigPath(const char *configPath);
	void setDerivativePath(const char *derivativePath);
	void setRepositoryPath(const char *repositoryPath);
	void setCataloguePath(const char *cataloguePath);
	void setWWWImagePath(const char* wwwImagePath);
	void setPicturePath(const char* picturePath);
	void setAllUsers(bool allUsers);
	void setConfigOption(const char* option);
	void setConfigValue(const char* value);
	void setConfigOptionBlock(const char* optionBlock);

	void setHomeEnv(bool b);

	AppOptions();
public:
	
	
	static AppOptions &get();
	virtual ~AppOptions();
	//bool initaliseConfig();
	//bool initalise(int argc, char **argv);
	AppOptions::CommandMode getCommandMode();
	const char *getName();
	const char *getCommand();
	const char *getComment();
	const char *getImageAddress();
	const char *getHomePath();
	const char *getWorkspacePath();
	const char *getMasterPath();
	const char *getConfigPath();
	const char *getDerivativePath();
	const char *getRepositoryPath();
	const char *getCataloguePath();
	const char* getWWWImagePath();
	const char* getPicturePath();
	const char* getConfigOption();
	const char* getConfigValue();
	const char* getConfigOptionBlock();
	const char* getOutputFile();
	const char* getTextOutputType();
	const char* getMediaSize();
	const char* getMediaPath();
	const char* getFromDate();
	const char* getToDate();

	bool isAllUsers();

	bool isConfiguratedOk() {
		return m_configured;
	}

	bool repair() {
		return m_repair;
	}

	bool validateMain() {
		return m_validateMain;
	}

	VerifyBackups getVerifyBackups() {
		return m_verifyBackups;
	}

	VerifyOperation getVerifyOperation() {
		return m_verifyOperation;
	}

	ShowOperation getShowOperation() {
		return m_showOperation;
	}

	bool getUsers() {
		return m_users;
	}
};

} /* namespace simplearchive */
#endif /* ADMINAPPOPTIONS_H_ */
