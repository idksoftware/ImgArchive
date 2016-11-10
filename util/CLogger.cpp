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

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdarg>
#include "CLogger.h"
#include "CIDKDate.h"
#include "UDPOut.h"
#include "LogName.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace std;
namespace simplearchive {
std::string CLogger::m_filename = "Log.txt";
CLogger *CLogger::m_this = NULL;
std::ofstream CLogger::m_logfile;
CLogger::Level CLogger::m_level;
std::string CLogger::m_logpath;
int CLogger::m_size = 10;
int CLogger::m_cursize = 0;
bool CLogger::m_isQuiet = false;
bool CLogger::m_isSilent = false;
int CLogger::m_lastCode;
std::string CLogger::m_lastMessage;

CLogger::CLogger() {
	//m_level = FINE;
	m_level = SUMMARY;
}

CLogger &CLogger::getLogger() {

	if (m_this == NULL) {
		m_this = new CLogger();
		//LogFilename logFilename(m_logpath.c_str());
		//logFilename.setMaxSize(m_size);
		//m_filename = logFilename.filename();
		makeFile();
	}
	return *m_this;
}

CLogger::~CLogger() {
	m_logfile.close();
}

void CLogger::makeFile() {
	LogName logName;
	m_filename = logName.makeName(m_logpath.c_str(), "", "log", 256);
	m_logfile.open(m_filename.c_str(), ios::out | ios::app);
	if (m_logfile.is_open() == false) {
		throw std::exception("Cannot open log file");
	}
}

void CLogger::log(int code, Level level, const char *format, ...) {
	if (m_size < m_cursize) {
		m_logfile.close();
		makeFile();
		m_cursize = 0;
	}
	if (!IsPrintable(level)) {
		return;
	}
	CIDKDate date;
	date.Now();
	char message[512];
	va_list args;
	va_start(args, format);
#ifdef _WIN32
	vsprintf_s(message, format, args);
#else
	vsprintf(message, format, args);
#endif
	m_lastMessage = message;
	m_lastCode = code;
	m_logfile << "\n" << date.Print() << ":\t";
	m_logfile << levelStr(level) << ":\t";
	m_logfile << message;
	m_cursize++;

	if (m_isSilent == false) {
		if (m_isQuiet == true && level >= Level::SUMMARY) {
			std::cout << message << '\n';
		}
		else {
			std::cout << message << '\n';
		}
	}
	std::stringstream str;
	str << setfill('0') << setw(3) << code << ":" << message;
	std::string udpMessage = str.str();
	UDPOut::out(udpMessage.c_str());
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
void CLogger::log(int code, Level level, const std::string &message) {

	if (!IsPrintable(level)) return;
	CIDKDate date;
	date.Now();
	m_logfile << "\n" << date.Print() << ":\t";
	m_logfile << message;
	UDPOut::out(message.c_str());
}

CLogger& CLogger::operator << (const std::string& message) {
	CIDKDate date;
	date.Now();
	m_logfile << "\n" << date.Print() << ":\t";
	m_logfile << message;
	UDPOut::out(message.c_str());
	return *this;
}

inline bool CLogger::IsPrintable(Level level) {
	if (level >= m_level) {
		return true;
	}
	return false;
}

const char *CLogger::levelStr(Level level) {
	switch (level) {
	case CLogger::TRACE: return "TRACE";
	case CLogger::FINE: return "FINE";
	case CLogger::INFO: return "INFO";
	case CLogger::SUMMARY: return "SUMMARY";
	case CLogger::WARNING: return "WARNING";
	case CLogger::ERR: return "ERROR";
	case FATAL: return "FATAL";
	}
	return "FATAL";
}

bool CLogger::setLevel(const std::string &s) {

	CLogger::Level level = Level::UNKNOWN;
	if (s.compare("TRACE")) {
		level = Level::TRACE;
	}
	else if (s.compare("FINE")) {
		level = Level::FINE;
	}
	else if (s.compare("INFO")) {
		level = Level::INFO;
	}
	else if (s.compare("SUMMARY")) {
		level = Level::SUMMARY;
	}
	else if (s.compare("WARNING")) {
		level = Level::WARNING;
	}
	else if (s.compare("ERROR")) {
		level = Level::ERR;
	}
	else if (s.compare("FATAL")) {
		level = Level::FATAL;
	}
	if (level == Level::UNKNOWN) {
		return false;
	}
	m_level = level;
	return true;
}

} /* namespace simplearchive */
