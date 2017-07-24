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

#include "CDate.h"
#include "cport.h"
#include <time.h>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

namespace simplearchive {

	CDate::CDate() {
		// TODO Auto-generated constructor stub

	}

	CDate::CDate(time_t t) {
		m_Time = t;
		time_t tt = m_Time;
		::localtime_p(m_timePtr, &tt);
	}

	CDate::CDate(const char *str) {
		std::string data = str;
		int delim1 = data.find_first_of('.');
		int delim2 = data.find_first_of(".", delim1 + 1);
		int delim3 = data.find_first_of(".", delim2 + 1);
		int delim4 = data.find_first_of(".", delim3 + 1);
		int delim5 = data.find_first_of(".", delim4 + 1);

		time_t rawtime;
		time(&rawtime);
		localtime_p(m_timePtr, &rawtime);
		std::string tmp;
		tmp = data.substr(0, delim1);
		m_timePtr.tm_year = strtol(tmp.c_str(), 0, 10);
		m_timePtr.tm_year = m_timePtr.tm_year - 1900;
		tmp = data.substr(delim1 + 1, delim2 - (delim1 + 1));
		m_timePtr.tm_mon = strtol(tmp.c_str(), 0, 10);
		m_timePtr.tm_mon = m_timePtr.tm_mon - 1;
		tmp = data.substr(delim2 + 1, delim3 - (delim2 + 1));
		m_timePtr.tm_mday = strtol(tmp.c_str(), 0, 10);
		tmp = data.substr(delim3 + 1, delim4 - (delim3 + 1));
		m_timePtr.tm_hour = strtol(tmp.c_str(), 0, 10);
		tmp = data.substr(delim4 + 1, delim5 - (delim4 + 1));
		m_timePtr.tm_min = strtol(tmp.c_str(), 0, 10);
		tmp = data.substr(delim5 + 1, data.length());
		m_timePtr.tm_sec = strtol(tmp.c_str(), 0, 10);
		m_Time = mktime(&m_timePtr);
	}

	CDate CDate::daysAgo(int days) {
		time_t tm = ::time(NULL);
		tm -= 24 * 3600 * days;
		CDate date(tm);
		return date;
	}

	CDate CDate::timeNow() {
		time_t tm = ::time(NULL);
		CDate now(tm);
		return now;
	}

	CDate::~CDate() {
		// TODO Auto-generated destructor stub
	}
	std::string CDate::current() {
		std::stringstream s;
		s << std::setw(4) << std::setfill('0') << getYear() << '.';
		s << std::setw(2) << std::setfill('0') << getMonth() << '.';
		s << std::setw(2) << std::setfill('0') << getDay() << '.';
		s << std::setw(2) << std::setfill('0') << getHour() << '.';
		s << std::setw(2) << std::setfill('0') << getMin() << '.';
		s << std::setw(2) << std::setfill('0') << getSec();
		return s.str();
		//return "2014/05/21 16:49:00";
	}

} /* namespace simplearchive */
