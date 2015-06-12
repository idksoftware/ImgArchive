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

#include "CFolderStatus.h"
#include <string>
#include <cstdlib>
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {
class CFolderStatusItem {
	typedef enum {
		CheckedIn, //
		CheckedOut,
		UnVersioned,	// Not versioned i.e. a changed image
						// with no versioned copy in the archive.
	} Status;
	Status m_status;
	std::string m_filename;

public:
	CFolderStatusItem();
	CFolderStatusItem(const char *dataString) {
		std::string m_data = dataString;
		int delim1 = m_data.find_first_of(':');
		int delim2 = m_data.find_first_of(delim1, ':');
		std::string crcStr = m_data.substr(0,delim1);
		int m_crc = strtol(crcStr.c_str(),NULL,16);
		std::string name = m_data.substr(delim1, delim2);
		std::string md5 = m_data.substr(delim2, m_data.length());
	}
};

class CFolderStatusContainer : public std::vector<CFolderStatusItem> {

public:

};


CFolderStatus::CFolderStatus() {
	// TODO Auto-generated constructor stub

}

CFolderStatus::~CFolderStatus() {
	// TODO Auto-generated destructor stub
}

bool update() {
	return true;
}

} /* namespace simplearchive */
