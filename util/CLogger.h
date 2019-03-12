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

class ReporterEvent {
public:
	enum class Status {
		Completed,
		Warning,
		Error,
		Fatal,
		Unkown
	};
	const char *statusString();
	static const char *statusString(ReporterEvent::Status status);
	std::string m_message;
	Status m_status;
	ReporterEvent(ReporterEvent::Status status, std::string &message);
};

typedef std::vector<ReporterEvent> StatusList;

class StatusReporter {
	std::shared_ptr<StatusList> m_list;
public:
	StatusReporter() : m_list(std::make_shared<StatusList>())
	{};
	static StatusReporter &get();
	void add(ReporterEvent::Status status, const char *format, ...);
	void add(ReporterEvent::Status status, std::string &msg);
	std::shared_ptr<StatusList> getList() { return m_list; };
};

class SummaryReporter {
	std::shared_ptr<StatusList> m_list;
	std::string m_summary;
	std::string m_result;
protected:
	int m_completed;
	int m_warning;
	int m_error;
	int m_fatal;
	int m_unknown;
	virtual bool doProcess();
	std::shared_ptr<StatusList> getList() { return m_list; };
	const char *getSummary() { return m_summary.c_str(); };
	const char *getResult() { return m_result.c_str(); };
public:
	SummaryReporter(std::shared_ptr<StatusList> list) : m_list(list), 
				m_completed(0), m_warning(0), m_error(0), m_fatal(0), m_unknown(0) {};

	bool process() {
		return doProcess();
	}

	SummaryReporter() = default;
	virtual ~SummaryReporter() = default;
	void setSummary(const char *s) { m_summary = s; };
	
	void setResult(const char *s) { m_result = s; };
	void toConsole();
};


typedef std::vector<std::string> LogBuffer;

class CLogger {
public:
	enum class Level {
		TRACE = 0,		// Trace debugging
		FINE = 1,		// Fine debugging
		INFO = 2,		// General information
		STATUS = 3,		// Status information
		SUMMARY = 4,	// Summary information
		WARNING = 5,	// Warning of incomplete operations
		ERR = 6,		// Failed operations
		FATAL = 7,		// Application cannot complete
		UNKNOWN = -1
	};
	

	void log(int code, Level level, const std::string &message);
	void log(int code, Level level, const char *format, ...);
	void CLogger::status(int cod, ReporterEvent::Status level, const char *format, ...);
	CLogger& operator << (const std::string& message);
	static CLogger &getLogger();
	
	/*
	static void Log(Level level, const std::string &message) {
		CLogger &Logger = getLogger();
		Logger.Log(level, message);
	}
	static void Log(Level level, const char *format, ...);
	*/

	Level getLogLevel() const {
		return m_logLevel;
	}
	static void processBuffer();

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

	static void setLogLevel(Level level) {
		m_logLevel = level;
	}

	static bool setLogLevel(const std::string &level) {
		return setLevel(m_logLevel, level);
	}

	static void setLogPath(const char *logpath);
	static void startLogging();

	static void Close() {
		if (m_this != nullptr) {
			m_this->m_logfile.close();
			m_this = nullptr;
		}
	}
	virtual ~CLogger();
private:

	CLogger();
	CLogger(const CLogger&) {};
	CLogger& operator = (const CLogger& ) { return *this; }
	bool IsLogOut(Level level);
	static bool CLogger::IsConsoleOut(Level level);
	static void makeFile();
	const char *levelStr(Level level);
	static bool m_isQuiet;
	static bool m_isSilent;
	static bool m_isOpen;
	static std::string m_filename;
	static const std::string m_Path;
	static std::unique_ptr<CLogger> m_this;
	static std::ofstream m_logfile;
	static Level m_logLevel;
	static Level m_consoleLevel;
	static Level m_isConsoleOut;
	static std::string m_logpath;
	static int m_size;
	static int m_cursize;
	static int m_lastCode;
	static std::string m_lastMessage;
	static std::unique_ptr<LogBuffer> m_startUpBuffer;
	static bool setLevel(CLogger::Level &level, const std::string &s);
	static CLogger::Level toLevel(const std::string &s);
	static const char *toString(CLogger::Level level);
	static CLogger::Level messageLevel(std::string message);
};

} /* namespace simplearchive */

