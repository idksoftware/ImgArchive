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
	typedef enum {
		CM_InitArchive, //* Initalise an archive with the default prameter
		CM_Show,        //* Show
		CM_Version,     //* show version
		CM_View,        //* View
		CM_Mirror,      //* Mirror
		CM_Archive,     //*
		CM_Validate,    //* Validate the archive.
        CM_Fix,         //* Fix the archive
		CM_Unknown
	} CommandMode;

	typedef enum {
		Workspace,		//* Initalise an archive with the default 
		Shadow,			//* Show
		Both			//* show version
	} Scope;

private:
	static AppOptions *m_this;
	static CommandMode m_commandMode;
	static std::string m_name;
	static std::string m_comment;
	static std::string m_imageAddress;
	static std::string m_homePath;
	static std::string m_workspacePath;
	static std::string m_shadowPath;
	static std::string m_configPath;
	static Scope m_scope;
	static bool m_repair;
	static bool m_users;
	bool m_error;
	/**
		@brief Set to true if no archive configuration found. 
	*/
	bool m_configured;
	void setCommandMode(const AppOptions::CommandMode mode);
	bool setCommandMode(const char *modeString);
	void setName(const char *name);
	void setHomePath(const char *homePath);
	void setWorkspacePath(const char *workspacePath);
	void setShadowPath(const char *shadowPath);
	void setConfigPath(const char *configPath);
	void setAllUsers(bool allUsers);
	AppOptions();
public:
	
	static AppOptions &get();

	virtual ~AppOptions();
	bool initaliseConfig();
	bool initalise(int argc, char **argv);
	AppOptions::CommandMode getCommandMode();
	const char *getName();
	const char *getCommand();
	const char *getComment();
	const char *getImageAddress();
	const char *getHomePath();
	const char *getWorkspacePath();
	const char *getShadowPath();
	const char *getConfigPath();
	bool isAllUsers();

	bool isConfiguratedOk() {
		return m_configured;
	}

	bool repair() {
		return m_repair;
	}

	Scope getScope() {
		return m_scope;
	}

	bool getUsers() {
		return m_users;
	}
};

} /* namespace simplearchive */
#endif /* ADMINAPPOPTIONS_H_ */
