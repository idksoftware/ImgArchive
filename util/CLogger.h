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

#ifndef CLOGGER_H_
#define CLOGGER_H_
#include <fstream>
#include <iostream>

namespace simplearchive {

class CLogger {
public:
	typedef enum {
		TRACE = 0,
		FINE = 1,
		INFO = 2,
		SUMMARY = 3,
		WARNING = 4,
		ERR = 5,
		FATAL = 6,
		UNKNOWN = -1
	} Level;
	

	void log(int code, Level level, const std::string &message);
	void log(int code, Level level, const char *format, ...);
	CLogger& operator << (const std::string& message);
	static CLogger &getLogger();

	/*
	static void Log(Level level, const std::string &message) {
		CLogger &Logger = getLogger();
		Logger.Log(level, message);
	}
	static void Log(Level level, const char *format, ...);
	*/

	Level getLevel() const {
		return m_level;
	}

	static const int getLastCode() {
		return m_lastCode;
	}
	static const char *getLastMessage() {
		return m_lastMessage.c_str();
	}
	static void setMaxSize(int size) {
		m_size = size;
	}

	static void setSilent(bool b = true) {
		m_isSilent = b;
	}

	static void setLevel(Level level) {
		m_level = level;
	}

	static bool setLevel(const std::string &level);

	static void setLogPath(const char *logpath) {
		m_logpath = logpath;
	}
	
	static void Close() {
		if (m_this != nullptr) {
			m_this->m_logfile.close();
			m_this = nullptr;
		}
	}

private:

	CLogger();
	CLogger(const CLogger&) {};
	CLogger& operator = (const CLogger& ) { return *this; }
	bool IsPrintable(Level level);
	static void makeFile();
	const char *levelStr(Level level);
	static bool m_isQuiet;
	static bool m_isSilent;
	static std::string m_filename;
	static const std::string m_Path;
	static CLogger* m_this;
	static std::ofstream m_logfile;
	static Level m_level;
	static std::string m_logpath;
	static int m_size;
	static int m_cursize;
	static int m_lastCode;
	static std::string m_lastMessage;
	virtual ~CLogger();
};

} /* namespace simplearchive */
#endif /* CLOGGER_H_ */
