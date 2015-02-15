/*
 * ImageHistory.h
 *
 *  Created on: Jun 2, 2014
 *      Author: wzw7yn
 */

#ifndef HISTORY_H_
#define HISTORY_H_
#include <vector>
#include "HistoryEvent.h"

namespace simplearchive {

#define HISTORY_FILE "history.dat"
class EventList;

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
	static History* m_this;
	static std::ofstream m_logfile;
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
};

} /* namespace simplearchive */
#endif /* HISTORY_H_ */
