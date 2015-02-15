/*
 * HistoryEvent.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: wzw7yn
 */
#include <string>
#include "HistoryEvent.h"

namespace simplearchive {

HistoryEvent::HistoryEvent() {
	m_event = ERROR;
}

HistoryEvent::~HistoryEvent() {
	// TODO Auto-generated destructor stub
}

const char *HistoryEvent::getString() const {
	switch (m_event) {
	case ADDED:
		return "added";
	case CHECKOUT:
		return "checkout";
	case CHECKIN:
		return "checkin";
	case EXPORT:
		return "export";
	case UNCHECKOUT:
		return "uncheckout";
	case ERROR:
		return "error";
	}
	return "error";
}

void HistoryEvent::set(const char *he) {
	std::string eventstr = he;
	if (eventstr.compare("add") == 0) {
		m_event = ADDED;
	} else if(eventstr.compare("checkout") == 0) {
		m_event = CHECKOUT;
	} else if(eventstr.compare("checkin") == 0) {
		m_event = CHECKIN;
	} else if(eventstr.compare("export") == 0) {
		m_event = EXPORT;
	}
	m_event = ERROR;

}

} /* namespace simplearchive */
