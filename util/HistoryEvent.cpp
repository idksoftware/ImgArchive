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

#include <string>
#include "HistoryEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

HistoryEvent::HistoryEvent() {
	m_event = ERROR;
}

HistoryEvent::~HistoryEvent() {
	// TODO Auto-generated destructor stub
}

const char *HistoryEvent::getString() const {
	switch (m_event) {
	case ADDED:
		return "added";
	case CHECKOUT:
		return "checkout";
	case CHECKIN:
		return "checkin";
	case EXPORT:
		return "export";
	case UNCHECKOUT:
		return "uncheckout";
	case ERROR:
		return "error";
	}
	return "error";
}

void HistoryEvent::set(const char *he) {
	std::string eventstr = he;
	if (eventstr.compare("add") == 0) {
		m_event = ADDED;
	} else if(eventstr.compare("checkout") == 0) {
		m_event = CHECKOUT;
	} else if(eventstr.compare("checkin") == 0) {
		m_event = CHECKIN;
	} else if(eventstr.compare("export") == 0) {
		m_event = EXPORT;
	}
	m_event = ERROR;

}

} /* namespace simplearchive */
