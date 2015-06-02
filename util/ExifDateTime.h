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

#ifndef EXIFDATETIME_H_
#define EXIFDATETIME_H_
#include <string>
namespace simplearchive {

class ExifDateTime {
	int m_sec;
	int m_min;
	int m_hour;
	int m_day;
	int m_month;
	int m_year;
	bool m_isOk;
	time_t m_timenum;
public:
	ExifDateTime();
	ExifDateTime(const char *datestr);
	
	virtual ~ExifDateTime();
	ExifDateTime &operator =(time_t time);

	std::string toString(); 
	std::string current();
	std::string toLogString();
	void setDateTime(time_t time);
	void now();

	int getDay() const {
		return m_day;
	}

	int getHour() const {
		return m_hour;
	}

	int getMin() const {
		return m_min;
	}

	int getMonth() const {
		return m_month;
	}

	int getSec() const {
		return m_sec;
	}

	int getYear() const {
		return m_year;
	}

	bool isOk() const {
		return m_isOk;
	}

	time_t getTime() const {
		return m_timenum;
	}
};

} /* namespace simplearchive */
#endif /* EXIFDATETIME_H_ */
