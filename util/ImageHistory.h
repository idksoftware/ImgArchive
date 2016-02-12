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
#include "ImageHistoryLog.h"

namespace simplearchive {

class ImageHistoryItem;
class HistoryEvent;
class EventList;
/**
This is the rolling system history log.
*/
class ImageHistory {
public:

private:

	bool read(const char *filepath);
	bool write(const char *filepath);
	EventList *m_eventList;
	
	static ImageHistory *m_this;
	static std::ofstream m_hstfile;
	static std::string m_primary;
	static std::string m_backup1;
	static std::string m_backup2;
	static std::string m_shadow;
	static std::string m_filename;
	ImageHistory(const ImageHistory&);
	ImageHistory& operator = (const ImageHistory&) { return *this; }
	ImageHistory();
	bool add(ImageHistoryItem &historyItem, const char *historyFile);
public:
	virtual ~ImageHistory();
	bool add(const char *filepath, int version, const char *comment, const HistoryEvent &he);
	bool add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he);
	std::auto_ptr<ImageHistoryLog> get();
	static ImageHistory &getImageHistory();
	static void setPath(const char *primaryPath, const char *shadowPath) {
		m_primary = primaryPath;
		m_shadow = shadowPath;
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
