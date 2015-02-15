/*
 * HistoryEvent.h
 *
 *  Created on: Jun 2, 2014
 *      Author: wzw7yn
 */

#ifndef HISTORYEVENT_H_
#define HISTORYEVENT_H_


namespace simplearchive {

class HistoryEvent {
public:
	typedef enum {
		ERROR = 1,
		ADDED,
		CHECKOUT,
		CHECKIN,
		UNCHECKOUT,
		EXPORT
	} Event;
	Event m_event;
public:

	const char *getString() const;
	Event get() {
		return m_event;
	}
	void set(const char *he);
	void set(Event &he) {
		m_event = he;
	}
	HistoryEvent();
	HistoryEvent(Event e) {
		m_event = e;
	}
	virtual ~HistoryEvent();
};

} /* namespace simplearchive */
#endif /* HISTORYEVENT_H_ */
