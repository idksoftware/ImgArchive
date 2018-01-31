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

#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>




namespace simplearchive {

typedef std::vector<std::string> LogBuffer;

class CLogger {
public:
	enum class Level {
		TRACE = 0,
		FINE = 1,
		INFO = 2,
		SUMMARY = 3,
		WARNING = 4,
		ERR = 5,
		FATAL = 6,
		UNKNOWN = -1
	};
	

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
		/*
		if (m_lastCode == 0) {
			// check logger for errors
			if (CLogger::getLastCode() != 0) {
				m_lastCode = CLogger::getLastCode();
			}
		}
		*/
		return m_lastCode;
	}
	static const char *getLastMessage() {
		/*
		if (CLogger::getLastCode() != 0) {
			m_lastCode = CLogger::
		}
		*/
		return m_lastMessage.c_str();
	}
	static void setMaxSize(int size) {
		m_size = size;
	}

	static void setSilent(bool b = false) {
		m_isSilent = b;
	}

	static void setConsoleLevel(Level level) {
		m_isConsoleOut = level;
	}

	static bool setConsoleLevel(const std::string &level) {
		return setLevel(m_isConsoleOut, level);
	}

	static void setLevel(Level level) {
		m_level = level;
	}

	static bool setLevel(const std::string &level) {
		return setLevel(m_level, level);
	}

	static void setLogPath(const char *logpath);

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
	bool CLogger::IsConsoleOut(Level level);
	static void makeFile();
	const char *levelStr(Level level);
	static bool m_isQuiet;
	static bool m_isSilent;
	static bool m_isOpen;
	static std::string m_filename;
	static const std::string m_Path;
	static CLogger* m_this;
	static std::ofstream m_logfile;
	static Level m_level;
	static Level m_isConsoleOut;
	static std::string m_logpath;
	static int m_size;
	static int m_cursize;
	static int m_lastCode;
	static std::string m_lastMessage;
	static std::unique_ptr<LogBuffer> m_startUpBuffer;
	static bool setLevel(CLogger::Level &level, const std::string &s);
	virtual ~CLogger();
};

} /* namespace simplearchive */

