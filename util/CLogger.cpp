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
#include <algorithm>
#include <cctype>
#include <functional>

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
std::unique_ptr<CLogger> CLogger::m_this(nullptr);
std::ofstream CLogger::m_logfile;
CLogger::Level CLogger::m_logLevel;
CLogger::Level CLogger::m_consoleLevel;
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
	m_logLevel = Level::SUMMARY;
	
	m_consoleLevel = Level::SUMMARY;
	m_isConsoleOut = Level::SUMMARY;
}

CLogger &CLogger::getLogger() {
	if (m_this.get() == nullptr)
	{
		m_this.reset(new CLogger());
		makeFile();
	}
	return *m_this;
}

CLogger::~CLogger() {
	m_logfile.close();
	m_isOpen = false;
	
}

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}



CLogger::Level CLogger::messageLevel(std::string message) {
	int spos = message.find("[");
	int epos = message.find("]", spos + 1);
	string levelStr = message.substr(spos+1, (epos-spos)-1);
	trim(levelStr);
	return toLevel(levelStr);
	
}

void CLogger::setLogPath(const char *logpath) {
	m_logpath = logpath;
}

void CLogger::startLogging() {
	m_isOpen = true;
	makeFile();
	processBuffer();
}

void CLogger::processBuffer() {
	for (auto i = m_startUpBuffer->begin(); i != m_startUpBuffer->end(); i++) {
		
		m_logfile << *i;
	
		if (m_isSilent == false) {
			std::string message = *i;
			CLogger::Level level = messageLevel(*i);
			if (IsConsoleOut(level)) {
				if (m_isQuiet) {
					std::cout << message << '\n';
				}
				
			}
			UDPOut::out(message.c_str());
		}
		
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
	if (!IsLogOut(level) && m_isOpen) {
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
	try {
		char message[1024];
		va_list args;
		va_start(args, format);
#ifdef _WIN32
		vsprintf_s(message, format, args);
#else
		vsprintf(message, format, args);
#endif
		va_end(args);
		m_lastMessage = message;
		m_lastCode = code;
		std::stringstream logstr;
		logstr << "\n" << setfill('0') << setw(6) << code << ": " << date.toLogString() << '.' << count << "\t";
		logstr << '[' << levelStr(level) << "]\t";
		logstr << message;
		if (m_isOpen) {
			if (IsLogOut(level)) {
				m_logfile << logstr.str();
			}
		}
		else {
			m_startUpBuffer->push_back(logstr.str());
		}
		m_cursize++;

		if (m_isSilent == false) {
			if (IsConsoleOut(level)) {
				if (m_isQuiet) {
					std::cout << message << '\n';
				}
				else {
					std::cout << setfill('0') << setw(6) << code << ": " << message << '\n';
				}
			}
		}

		std::stringstream strudp;
		strudp << setfill('0') << setw(6) << code << ":" << message;
		std::string udpMessage = strudp.str();
		UDPOut::out(udpMessage.c_str());
	}
	catch (exception e) {
		printf("logger crashed");
		exit(-1);
	}
	
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
	
	if (!IsLogOut(level)) return;
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

inline bool CLogger::IsLogOut(Level level) {
	if (level >= m_logLevel) {
		return true;
	}
	return false;
}

inline bool CLogger::IsConsoleOut(Level level) {
	if (level >= m_consoleLevel) {
		return true;
	}
	return false;
}

const char *CLogger::toString(CLogger::Level level) {
	switch (level) {
	case CLogger::Level::TRACE: return    "TRACE";
	case CLogger::Level::FINE: return     "FINE";
	case CLogger::Level::INFO: return     "INFO";
	case CLogger::Level::SUMMARY: return  "SUMMARY";
	case CLogger::Level::WARNING: return  "WARNING";
	case CLogger::Level::ERR: return      "ERROR";
	case CLogger::Level::FATAL: return    "FATAL";
	case CLogger::Level::UNKNOWN: return  "FATAL";
	}
	return "FATAL";
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

CLogger::Level CLogger::toLevel(const std::string &s) {
	CLogger::Level level;
	if (s.compare("TRACE") == 0) {
		level = Level::TRACE;
	}
	else if (s.compare("FINE") == 0) {
		level = Level::FINE;
	}
	else if (s.compare("INFO") == 0) {
		level = Level::INFO;
	}
	else if (s.compare("SUMMARY") == 0) {
		level = Level::SUMMARY;
	}
	else if (s.compare("WARNING") == 0) {
		level = Level::WARNING;
	}
	else if (s.compare("ERROR") == 0) {
		level = Level::ERR;
	}
	else if (s.compare("FATAL") == 0) {
		level = Level::FATAL;
	} else {
		level = Level::UNKNOWN;
	}
	return level;
}


bool CLogger::setLevel(CLogger::Level &level, const std::string &s) {

	level = toLevel(s);
	if (level == Level::UNKNOWN) {
		return false;
	}
	
	return true;
}

} /* namespace simplearchive */
