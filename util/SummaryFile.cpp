/*
 * SummaryFile.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: wzw7yn
 */



#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdarg>
#include "SummaryFile.h"
#include "EXifDateTime.h"

//using namespace std;
namespace simplearchive {
std::string SummaryFile::m_briefFilename;
std::string SummaryFile::m_detailedFilename;
std::auto_ptr<SummaryFile> SummaryFile::m_this(0);
std::ofstream SummaryFile::m_summaryFile;
std::ofstream SummaryFile::m_detailedFile;

SummaryFile::SummaryFile() {
	m_level = SF_DETAILED;
	m_summaryFile.open(m_briefFilename.c_str(), std::ios::out);
	m_detailedFile.open(m_detailedFilename.c_str(), std::ios::out);
	m_added = 0;
	m_rejected = 0;
	m_duplicates = 0;
	m_exported = 0;
	m_checkedIn = 0;
	m_chectedOut = 0;
}

SummaryFile &SummaryFile::getSummaryFile() {

	if (!m_this.get()) {
		m_this.reset(new SummaryFile());

	}
	return *(m_this.get());
}

SummaryFile::~SummaryFile() {

	m_summaryFile.flush();
	m_detailedFile.flush();
	m_summaryFile.close();
	m_detailedFile.close();
}

void SummaryFile::doMessage(Level level, Action action, const char *message) {

	ExifDateTime dateNow;
	dateNow.now();
	if (level == SF_BRIEF) {
		m_summaryFile << "\n" << dateNow.current() << ":\t";
		m_summaryFile << actionStr(action) << ":\t";
		m_summaryFile << message;
	}
	m_detailedFile << "\n" << dateNow.current() << ":\t";
	m_detailedFile << actionStr(action) << ":\t";
	m_detailedFile << message;

	switch(action) {
	case SF_DUPLICATE:		m_duplicates++; break;
	case SF_ADDED:			m_added++; break;
	case SF_REJECTED:		m_rejected++; break;
	case SF_EXPORTED:		m_exported++; break;
	case SF_CHECKED_IN:		m_checkedIn++; break;
	case SF_CHECKED_OUT:	m_chectedOut++; break;
	case SF_COMMENT:	break;
	case SF_TOTAL:		break;
	}


}

void SummaryFile::doSummary() {
	char message[256];

	sprintf(message, "Added: %d", m_added);
	doMessage(SF_BRIEF, SF_TOTAL, message);
	sprintf(message, "Rejected: %d", m_rejected);
	doMessage(SF_BRIEF, SF_TOTAL, message);
	sprintf(message, "Duplicates: %d", m_duplicates);
	doMessage(SF_BRIEF, SF_TOTAL, message);
	sprintf(message, "Exported: %d", m_exported);
	doMessage(SF_BRIEF, SF_TOTAL, message);
	sprintf(message, "Checked in: %d", m_checkedIn);
	doMessage(SF_BRIEF, SF_TOTAL, message);
	sprintf(message, "Checked out: %d", m_chectedOut);
	doMessage(SF_BRIEF, SF_TOTAL, message);

}



void SummaryFile::log(Level level, Action action, const char *format, ...) {



	char message[256];
	va_list args;
	va_start(args, format);
	vsprintf(message, format, args);
	doMessage(level, action, message);
	va_end(args);

}

void SummaryFile::log(Level level, Action action, const std::string &message) {
	doMessage(level, action, message.c_str());
}

SummaryFile& SummaryFile::operator << (const std::string& message) {
	ExifDateTime dateNow;
	dateNow.now();
	m_summaryFile << "\n" << dateNow.current() << ":\t";
	m_summaryFile << message;
	return *this;
}

const char *SummaryFile::actionStr(Action action) {
	switch(action) {
	case SF_DUPLICATE:		return "Duplicate";
	case SF_ADDED:			return "Added";
	case SF_REJECTED:		return "Rejected";
	case SF_EXPORTED:		return "Exported";
	case SF_CHECKED_IN:		return "CheckedIn";
	case SF_CHECKED_OUT:	return "ChectedOut";
	case SF_COMMENT:		return "Comment";
	case SF_TOTAL:			return "Total";
	}
}

const char *SummaryFile::levelStr() {
	switch (m_level) {
	case SF_DETAILED: return "Detailed";
	case SF_BRIEF: return "Brief";

	}
}

} /* namespace simplearchive */
