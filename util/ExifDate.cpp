/*
 * ExifDate.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: wzw7yn
 */
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include "ExifDate.h"

namespace simplearchive {

ExifDate::ExifDate() {
	now();
}

ExifDate::ExifDate(const char *str) {
	m_isOk = true;
	std::string datestr = str;
	int s = 0;
	int e = datestr.find_first_of(':');
	if (e != 4) {
		now();
		m_isOk = false;
		return;
	}
	std::string numstr = datestr.substr(s, e);
	m_year = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.find_first_of(':', s);
	numstr = datestr.substr(s, e-s);
	m_month = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.length();
	numstr = datestr.substr(s, e-s);
	m_day = strtol(numstr.c_str(), NULL, 10);
	time(&m_timenum);
	struct tm *timeinfo = gmtime(&m_timenum);
	timeinfo->tm_year = m_year - 1900;
	timeinfo->tm_mon = m_month - 1;
	timeinfo->tm_mday = m_day;
	m_timenum = mktime(timeinfo);
	m_isOk = true;
}

ExifDate::ExifDate(int y, int m, int d) {

	struct tm timeinfo;

	m_year = y;
	timeinfo.tm_year = y - 1900;
	m_month = m;
	timeinfo.tm_mon = m - 1;
	m_day = d;
	timeinfo.tm_mday = d;
	m_timenum = mktime(&timeinfo);
	m_isOk = true;
}

ExifDate::ExifDate(time_t time) {
	m_timenum = time;
	struct tm *timeinfo = localtime(&m_timenum);

	m_year = timeinfo->tm_year + 1900;
	m_month = timeinfo->tm_mon + 1;
	m_day = timeinfo->tm_mday;
	
	m_isOk = true;
}

ExifDate::~ExifDate() {
	// TODO Auto-generated destructor stub
}

void ExifDate::now() {
	time(&m_timenum);
	struct tm *timeinfo = localtime(&m_timenum);

	m_year = timeinfo->tm_year + 1900;
	m_month = timeinfo->tm_mon + 1;
	m_day = timeinfo->tm_mday;

	m_isOk = true;
}

std::string ExifDate::toString() {
	std::stringstream tmp;
	tmp << m_year << ':' << m_month << ':' << m_day;

	return std::string(tmp.str());

}

std::string ExifDate::toFileString() {
	std::stringstream s;
	s << std::setw(4) << std::setfill('0') << getYear() << '.';
	s << std::setw(2) << std::setfill('0') << getMonth() << '.';
	s << std::setw(2) << std::setfill('0') << getDay();
	return std::string(s.str());

}


ExifDate& ExifDate::operator=(ExifDate& d) {
	m_timenum = d.m_timenum;
	m_year = d.m_year;
	m_month = d.m_month;
	m_day = d.m_day;
	m_isOk = d.m_isOk;
	return *this;
}

ExifDate& ExifDate::operator=(ExifDateTime& d) {
	m_timenum = d.getTime();
	m_year = d.getYear();
	m_month = d.getMonth();
	m_day = d.getDay();
	m_isOk = d.isOk();
	return *this;
}

} /* namespace simplearchive */
