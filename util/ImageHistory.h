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

class HistoryEvent;
class EventList;

class ImageHistory {
public:

private:

	bool read(const char *filepath);
	bool write(const char *filepath);
	EventList *m_eventList;
	std::string m_datapath;
public:
	ImageHistory(const char *datapath);
	virtual ~ImageHistory();
	bool add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he);
	std::auto_ptr<ImageHistoryLog> get();

};

} /* namespace simplearchive */
#endif /* IMAGEHISTORY_H_ */
