/*
 * CLogger.cpp
 *
 *  Created on: May 21, 2014
 *      Author: wzw7yn
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdarg>
#include "CLogger.h"
#include "CIDKDate.h"
//#include "LogFilename.h"
#include "LogName.h"

using namespace std;
namespace simplearchive {
std::string CLogger::m_filename = "Log.txt";
CLogger *CLogger::m_this = NULL;
std::ofstream CLogger::m_logfile;
CLogger::Level CLogger::m_level;
std::string CLogger::m_logpath;
int CLogger::m_size = 10;


CLogger::CLogger() {
	//m_level = FINE;
	m_level = INFO;
}

CLogger &CLogger::getLogger() {

	if (m_this == NULL) {
		m_this = new CLogger();
		//LogFilename logFilename(m_logpath.c_str());
		//logFilename.setMaxSize(m_size);
		//m_filename = logFilename.filename();
		LogName logName;
		m_filename = logName.makeName(m_logpath.c_str(), "", "log", 256);
		m_logfile.open(m_filename.c_str(), ios::out | ios::app);
		if (m_logfile.is_open() == false) {
			throw std::exception("Cannot open log file");
		}
	}
	return *m_this;
}

CLogger::~CLogger() {
	m_logfile.close();
}

void CLogger::log(Level level, const char *format, ...) {

	if (!IsPrintable(level)) return;
	CIDKDate date;
	date.Now();
	char message[256];
	va_list args;
	va_start(args, format);
#ifdef WIN32
	vsprintf_s(message, format, args);
#else
	vsprintf(message, format, args);
#endif

	m_logfile << "\n" << date.Print() << ":\t";
	m_logfile << levelStr() << ":\t";
	m_logfile << message;

	std::cout << "\n" << date.Print() << ":\t";
	std::cout << levelStr() << ":\t";
	std::cout << message;

	va_end(args);

}

/*
void CLogger::Log(Level level, const char *format, ...) {
	CLogger &Logger = getLogger();

	char message[256];
	va_list args;
	va_start(args, format);
	vsprintf(message, format, args);
	va_end(args);
	Logger.Log(level, message);
}
*/
void CLogger::log(Level level, const std::string &message) {

	if (!IsPrintable(level)) return;
	CIDKDate date;
	date.Now();
	m_logfile << "\n" << date.Print() << ":\t";
	m_logfile << message;
}

CLogger& CLogger::operator << (const std::string& message) {
	CIDKDate date;
	date.Now();
	m_logfile << "\n" << date.Print() << ":\t";
	m_logfile << message;
	return *this;
}

inline bool CLogger::IsPrintable(Level level) {
	if (level >= m_level) {
		return true;
	}
	return false;
}

const char *CLogger::levelStr() {
	switch (m_level) {
	case TRACE: return "TRACE";
	case FINE: return "FINE";
	case INFO: return "INFO";
	case WARNING: return "WARNING";
	case ERROR: return "ERROR";
	case FATAL: return "FATAL";
	}
	return "FATAL";
}

} /* namespace simplearchive */
