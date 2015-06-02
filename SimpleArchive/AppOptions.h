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
		CM_InitArchive,
		CM_Import,
		CM_Export,
		CM_Checkout,
		CM_Checkin,
		CM_UnCheckout,
		CM_Version,
		CM_View,
		CM_Mirror,
		CM_Uncheckin,
		CM_Archive,
		CM_Validate,
		CM_Unknown
	} CommandMode;
private:
	static AppOptions *m_this;
	static CommandMode m_commandMode;
	static std::string m_name;
	static std::string m_comment;
	static std::string m_imageAddress;
	bool m_error;
	void setCommandMode(const AppOptions::CommandMode mode);
	bool setCommandMode(const char *modeString);
	void setName(const char *name);
	AppOptions();
public:
	
	static AppOptions &get();

	virtual ~AppOptions();

	bool initalise(int argc, char **argv);
	AppOptions::CommandMode getCommandMode();
	const char *getName();
	const char *getCommand();
	const char *getComment();
	const char *getImageAddress();
	
};

} /* namespace simplearchive */
#endif /* ADMINAPPOPTIONS_H_ */
