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

#ifndef IMAGEHISTORY_H_
#define IMAGEHISTORY_H_
#include <vector>
#include <memory>
//#include "ImageHistoryLog.h"



namespace simplearchive {

class ImageHistoryItem;
class HistoryEvent;
class EventList;
/**
This is the rolling system history log.
*/
class SystemHistory {
public:

private:

	bool read(const char *filepath);
	bool write(const char *filepath);
	EventList *m_eventList;
	
	
	static std::ofstream m_hstfile;
	static std::string m_primary;
	static std::string m_currentFilename;	// This is the rolling history
	static std::string m_backup1;
	static std::string m_backup2;
	static std::string m_index;		// This is the history for each image
	static std::string m_filename;

	SystemHistory(const SystemHistory&);
	SystemHistory& operator = (const SystemHistory&) { return *this; }
	
	bool add(ImageHistoryItem &historyItem, const char *historyFile);
public:
	SystemHistory();
	virtual ~SystemHistory();
	bool add(const char *filepath, int version, const char *comment, const HistoryEvent &he);
	bool add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he);
	std::unique_ptr<ImageHistoryItem> get();
	bool SystemHistory::init();

	static void setPath(const char *primaryPath, const char *indexPath) {
		m_primary = primaryPath;
		m_index = indexPath;
	}
	static void setBackup1Path(const char *path) {
		m_backup1 = path;
	}
	static void setBackup2Path(const char *path) {
		m_backup2 = path;
	}
};

} /* namespace simplearchive */
#endif /* IMAGEHISTORY_H_ */
