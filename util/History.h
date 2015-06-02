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

#ifndef HISTORY_H_
#define HISTORY_H_
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


class History {

private:

	bool read(const char *filepath);
	bool write(const char *filepath);
	EventList *m_eventList;

	History();

	History(const History&);
	History& operator = (const History& ) { return *this; }
	static bool m_isOpen;
	static std::string m_filename;
	static std::auto_ptr<History> m_this;
	static std::ofstream m_logfile;
	static std::string m_folder;
	bool readLog(const char *logFile, HistoryLog &historyLog);
public:
	static void setPath(const char *path);
	static History &getHistory();
	virtual ~History();
	bool add(const char *filename, const char *version, const char *comment, const HistoryEvent &he);
	/*
	bool add(const HistoryEvent &he) {
		return true;
	}
	*/
	bool add() {
		return true;
	}

	//int getHistory(CDate &from, CDate &to);
	std::auto_ptr<HistoryLog> getEntries(int daysAgo);
	//std::string getHistory(int from, int to);
};

} /* namespace simplearchive */
#endif /* HISTORY_H_ */
