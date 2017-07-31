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



#include "siaglobal.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include "SAUtils.h"
#include "SummaryFile.h"
#include "ExifDateTime.h"
#include "UDPOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

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
	std::stringstream udpout;
	udpout << dateNow.current() << ":\t";
	udpout << actionStr(action) << ":\t";
	udpout << message;
	UDPOut::out(udpout.str().c_str());

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
	std::string message;

	message = SAUtils::sprintf("Added: %d", m_added);
	doMessage(SF_BRIEF, SF_TOTAL, message.c_str());
	message = SAUtils::sprintf("Rejected: %d", m_rejected);
	doMessage(SF_BRIEF, SF_TOTAL, message.c_str());
	message = SAUtils::sprintf("Duplicates: %d", m_duplicates);
	doMessage(SF_BRIEF, SF_TOTAL, message.c_str());
	message = SAUtils::sprintf("Exported: %d", m_exported);
	doMessage(SF_BRIEF, SF_TOTAL, message.c_str());
	message = SAUtils::sprintf("Checked in: %d", m_checkedIn);
	doMessage(SF_BRIEF, SF_TOTAL, message.c_str());
	message = SAUtils::sprintf("Checked out: %d", m_chectedOut);
	doMessage(SF_BRIEF, SF_TOTAL, message.c_str());

}



void SummaryFile::log(Level level, Action action, const char *format, ...) {

	char message[256];
	va_list args;
	va_start(args, format);
	vsnprintf(message, 256, format, args);
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
	return "Unknown";
}

const char *SummaryFile::levelStr() {
	switch (m_level) {
	case SF_DETAILED: return "Detailed";
	case SF_BRIEF: return "Brief";

	}
	return "Unknown";
}

} /* namespace simplearchive */
