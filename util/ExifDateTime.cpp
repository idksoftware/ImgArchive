/*
 * ExifDateTime.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: wzw7yn
 */
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include "ExifDateTime.h"

namespace simplearchive {

ExifDateTime::ExifDateTime() {
	now();
}

ExifDateTime::ExifDateTime(const char *str) {
	std::string datestr = str;
	int s = 0;
	int e = datestr.find_first_of(':');
	std::string numstr = datestr.substr(s, e);
	m_year = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.find_first_of(':', s);
	numstr = datestr.substr(s, e-s);
	m_month = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.find_first_of(' ', s);
	numstr = datestr.substr(s, e-s);
	m_day = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.find_first_of(':', s);
	numstr = datestr.substr(s, e-s);
	m_hour = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.find_first_of(':', s);
	numstr = datestr.substr(s, e-s);
	m_min = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.length();
	numstr = datestr.substr(s, e-s);
	m_sec = strtol(numstr.c_str(), NULL, 10);
	time(&m_timenum);
	struct tm *timeinfo = gmtime(&m_timenum);
	timeinfo->tm_year = m_year - 1900;
	timeinfo->tm_mon = m_month - 1;
	timeinfo->tm_mday = m_day;
	timeinfo->tm_hour = m_hour;
	timeinfo->tm_min = m_min;
	timeinfo->tm_sec = m_sec;
	m_timenum = mktime(timeinfo);
	m_isOk = true;
}

ExifDateTime::~ExifDateTime() {
	// TODO Auto-generated destructor stub
}

ExifDateTime &ExifDateTime::operator =(time_t time) {
	setDateTime(time);
	return *this;
}

void ExifDateTime::now() {
	time(&m_timenum);
	setDateTime(m_timenum);
}

void ExifDateTime::setDateTime(time_t time) {

	struct tm *timeinfo = localtime(&time);

	m_year = timeinfo->tm_year + 1900;
	m_month = timeinfo->tm_mon + 1;
	m_day = timeinfo->tm_mday;
	m_hour = timeinfo->tm_hour;
	m_min = timeinfo->tm_min;
	m_sec = timeinfo->tm_sec;
	m_isOk = true;
}

std::string ExifDateTime::toString() {
	std::stringstream tmp;
	tmp << m_year << ':' << m_month << ':' << m_day << ' ';
	tmp << m_hour << ':' << m_min << ':' << m_sec;
	return std::string(tmp.str());

}

std::string ExifDateTime::current() {
	std::stringstream s;
	s << std::setw(4) << std::setfill('0') << getYear() << '.';
	s << std::setw(4) << std::setfill('0') << getMonth() << '.';
	s << std::setw(4) << std::setfill('0') << getDay() << '.';
	s << std::setw(4) << std::setfill('0') << getHour() << '.';
	s << std::setw(4) << std::setfill('0') << getMin() << '.';
	s << std::setw(4) << std::setfill('0') << getSec();
	return s.str();
	//return "2014/05/21 16:49:00";
}

std::string ExifDateTime::toLogString() {
	std::stringstream tmp;
	tmp << m_year << '.' << m_month << '.' << m_day << ' ';
	tmp << m_hour << '.' << m_min << '.' << m_sec;
	return std::string(tmp.str());

}

} /* namespace simplearchive */
