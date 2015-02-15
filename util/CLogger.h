/*
 * CLogger.h
 *
 *  Created on: May 21, 2014
 *      Author: wzw7yn
 */

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
		WARNING = 3,
		ERROR = 4,
		FATAL = 5,
	} Level;
	

	void log(Level level, const std::string &message);
	void log(Level level, const char *format, ...);
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

	static void setMaxSize(int size) {
		m_size = size;
	}

	static void setLevel(Level level) {
		m_level = level;
	}

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
	const char *levelStr();
	static std::string m_filename;
	static const std::string m_Path;
	static CLogger* m_this;
	static std::ofstream m_logfile;
	static Level m_level;
	static std::string m_logpath;
	static int m_size;
	virtual ~CLogger();
};

} /* namespace simplearchive */
#endif /* CLOGGER_H_ */
