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
#include "SAUtils.h"
#include "CLogger.h"
#include "ExifDateTime.h"
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
CLogger::Level CLogger::m_isConsoleOut;
std::string CLogger::m_logpath;
int CLogger::m_size = 10000;
int CLogger::m_cursize = 0;
bool CLogger::m_isQuiet = false;
bool CLogger::m_isSilent = false;
bool CLogger::m_isOpen = false;
int CLogger::m_lastCode;
std::string CLogger::m_lastMessage;
std::unique_ptr<LogBuffer> CLogger::m_startUpBuffer;

class LoggBuffer : public std::vector<std::string> {

};

CLogger::CLogger() {
	m_startUpBuffer = make_unique<LogBuffer>();
	m_level = Level::FINE;
	//m_level = Level::SUMMARY;
	//m_isConsoleOut = Level::FINE;
	m_isConsoleOut = Level::SUMMARY;
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
	m_isOpen = false;
	
}

void CLogger::setLogPath(const char *logpath) {
	m_logpath = logpath;
	m_isOpen = true;
	makeFile();
	for (auto i = m_startUpBuffer->begin(); i != m_startUpBuffer->end(); i++) {
		m_logfile << *i;
	}
}

void CLogger::makeFile() {
	LogName logName;
	if (m_isOpen == false) {
		return;
	}
	m_filename = logName.makeName(m_logpath.c_str(), "", "log", 256);
	m_logfile.open(m_filename.c_str(), ios::out | ios::app);
	if (m_logfile.is_open() == false) {
		throw SIAAppException("Cannot open log file");
	}
}

ExifDateTime last;
int count = 0;

void CLogger::log(int code, Level level, const char *format, ...) {
	if (m_size < m_cursize) {
		m_logfile.close();
		makeFile();
		m_cursize = 0;
	}
	// Return if the message is to low leval to be include in the log, UDP or terminal.
	if (!IsPrintable(level)) {
		return;
	}

	ExifDateTime date;
	date.now();
	if (last.getTime() == date.getTime()) {
		count++;
	}
	else {
		count = 1;
	}
	last.now();
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
	std::stringstream logstr;
	logstr << "\n" << date.toLogString() << '.' << count << "\t";
	logstr << '[' << levelStr(level) << "]\t";
	logstr << message;
	if (m_isOpen) {
		m_logfile << logstr.str();
	}
	else {
		m_startUpBuffer->push_back(logstr.str());
	}
	m_cursize++;

	if (m_isSilent == false) {
		if (m_isQuiet == true && IsConsoleOut(level)) {
			std::cout << message << '\n';
		}
		else {
			std::cout << setfill('0') << setw(3) << code << ": " << message << '\n';
		}
	}
	std::stringstream strudp;
	strudp << setfill('0') << setw(3) << code << ":" << message;
	std::string udpMessage = strudp.str();
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
	ExifDateTime date;
	date.now();
	std::stringstream logMessage;
	logMessage << "\n" << date.toLogString() << ":\t";
	logMessage << message;
	if (m_isOpen) {
		m_logfile << logMessage.str();
	}
	else {

	}
	UDPOut::out(message.c_str());
}

CLogger& CLogger::operator << (const std::string& message) {
	ExifDateTime date;
	date.now();
	m_logfile << "\n" << date.toLogString() << ":\t";
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

inline bool CLogger::IsConsoleOut(Level level) {
	if (level >= m_level) {
		return true;
	}
	return false;
}

const char *CLogger::levelStr(CLogger::Level level) {
	switch (level) {
	case CLogger::Level::TRACE: return    "     TRACE";
	case CLogger::Level::FINE: return     "     FINE ";
	case CLogger::Level::INFO: return     "    INFO  ";
	case CLogger::Level::SUMMARY: return  "   SUMMARY";
	case CLogger::Level::WARNING: return  "  WARNING ";
	case CLogger::Level::ERR: return      " ERROR    ";
	case CLogger::Level::FATAL: return    "FATAL     ";
	case CLogger::Level::UNKNOWN: return  "FATAL     ";
	}
	return "FATAL";
}

bool CLogger::setLevel(CLogger::Level &level, const std::string &s) {

	
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
	
	return true;
}

} /* namespace simplearchive */
