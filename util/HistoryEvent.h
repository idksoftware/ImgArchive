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
	static const char getChar(HistoryEvent::Event evt);
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
