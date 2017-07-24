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

#include <string>
#include <vector>
#include <memory>
#include "HistoryEvent.h"
#include "CDate.h"

namespace simplearchive {

#define HISTORY_FILE "history.dat"
class EventList;
class CDate;
class HistoryLog;

/**
	This is the Image only log for one image.
	The history normaly will end in <imagename>.<ext>.hst
*/
class HistoryItem;
class ArchivePath;
class ImageHistory {

private:
	static std::string m_workspace;
	static std::string m_index;
	
	ImageHistory(const ImageHistory&);
	ImageHistory& operator = (const ImageHistory&) { return *this; };
	
	bool writeLog(HistoryItem &item, const char *path);
	bool readLog(const char *logFile, HistoryLog &historyLog);
public:

	ImageHistory();
	virtual ~ImageHistory();
	static void setPath(const char *workspacePath, const char *indexPath) {
		m_workspace = workspacePath;
		m_index = indexPath;
	}
	bool init();
	
	bool add(const char *filename, const char *comment);
	bool add(const char *filename, int version, const char *comment, const HistoryEvent &he);
	
	bool add(const char *filename, const char *version, const char *comment, const HistoryEvent &he);
	
	std::shared_ptr<HistoryLog> getEntries(int daysAgo);
	
};

} /* namespace simplearchive */

