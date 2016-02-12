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

#include "SetEnv.h"
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

SetEnv::SetEnv() {
	// TODO Auto-generated constructor stub

}

SetEnv::~SetEnv() {
	// TODO Auto-generated destructor stub
}

void SetEnv::process() {
	for (std::vector<EnvItem>::iterator i = begin(); i != end(); i++) {
		EnvItem &data = *i;
#ifdef _WIN32
		//::SetEnvironmentVariable(data.getName().c_str(), data.getValue().c_str());
		std::string tmp = data.getName() + '=' + data.getValue().c_str();
		_putenv(tmp.c_str());

#else
		setenv(data.getName().c_str(),data.getValue().c_str(), true);
#endif
	}
}

} /* namespace simplearchive */
