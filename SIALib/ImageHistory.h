/*
 * ImageHistory.h
 *
 *  Created on: Jun 2, 2014
 *      Author: wzw7yn
 */

#ifndef IMAGEHISTORY_H_
#define IMAGEHISTORY_H_
#include <vector>


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

};

} /* namespace simplearchive */
#endif /* IMAGEHISTORY_H_ */
