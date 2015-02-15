/*
 * SummaryFile.h
 *
 *  Created on: Feb 5, 2015
 *      Author: wzw7yn
 */

#ifndef SUMMARYFILE_H_
#define SUMMARYFILE_H_

#include <string>
#include <fstream>
#include <iostream>
#include <memory>

namespace simplearchive {



class SummaryFile {
public:
	typedef enum {
		SF_DETAILED = 0,
		SF_BRIEF = 1,
	} Level;

	typedef enum {
		SF_DUPLICATE = 0,
		SF_ADDED = 1,
		SF_REJECTED = 2,
		SF_EXPORTED = 3,
		SF_CHECKED_IN = 4,
		SF_CHECKED_OUT = 5,
		SF_COMMENT = 6,
		SF_TOTAL = 7
	} Action;


	void log(Level level, Action action, const std::string &message);
	void log(Level level, Action action, const char *format, ...);
	SummaryFile& operator << (const std::string& message);
	static SummaryFile &getSummaryFile();

	Level getLevel() const {
		return m_level;
	}

	void setLevel(Level level) {
		m_level = level;
	}

	const char *levelStr();

	static void setPath(const char *str) {
		m_briefFilename = str;
		m_briefFilename += "/brief.txt";

		m_detailedFilename = str;
		m_detailedFilename += "/detailed.txt";

	}

	int getAdded() const {
		return m_added;
	}

	int getCheckedIn() const {
		return m_checkedIn;
	}

	int getChectedOut() const {
		return m_chectedOut;
	}

	int getDuplicates() const {
		return m_duplicates;
	}

	int getExported() const {
		return m_exported;
	}
	void doSummary();
	virtual ~SummaryFile();
private:
	Level m_level;
	SummaryFile();

	SummaryFile& operator = (const SummaryFile& ) { return *this; }

	static std::string m_briefFilename;
	static std::string m_detailedFilename;
	static std::auto_ptr<SummaryFile> m_this;
	static std::ofstream m_summaryFile;
	static std::ofstream m_detailedFile;


	const char *actionStr(Action action);
	void doMessage(Level level, Action action, const char *message);


	int m_added;
	int m_rejected;
	int m_duplicates;
	int m_exported;
	int m_checkedIn;
	int m_chectedOut;

};


} /* namespace simplearchive */
#endif /* SUMMARYFILE_H_ */
