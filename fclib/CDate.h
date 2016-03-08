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

#ifndef CDATE_H_
#define CDATE_H_
#include <time.h>
#include <ctime>
#include <string>

namespace simplearchive {

class CDate {
	time_t m_Time;
	tm m_timePtr;
public:
	CDate();
	CDate(time_t t);
	CDate(const char *str);
	virtual ~CDate();
	static CDate timeNow();
	int getSec() const {
		return m_timePtr.tm_sec;
	}

	int getMin() const {
		return m_timePtr.tm_min;
	}

	int getHour() const {
		return m_timePtr.tm_hour;
	}

	int getDay() const {
		return m_timePtr.tm_mday;
	}

	int getMonth() const {
		return m_timePtr.tm_mon + 1;
	}

	int getYear() const {
		return m_timePtr.tm_year + 1900;
	}
	std::string current();
	static CDate daysAgo(int days);
};

} /* namespace simplearchive */
#endif /* CDATE_H_ */
