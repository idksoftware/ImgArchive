/*
 * ImageHistory.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: wzw7yn
 */

#include <string>
#include <vector>
#include <fstream>

#include <streambuf>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdarg>
#include "History.h"
#include "LogName.h"
#include "ExifDateTime.h"

namespace simplearchive {
class HistoryItem {
	std::string m_comment;
	std::string m_date;
	std::string m_filepath;
	std::string m_event;
	std::string m_data;
	std::string m_version;
public:
	HistoryItem();
	/*
	 * 14.06.12.12.16.12:56:first checkin:checkin
	 */
	HistoryItem(const char *dataString) {
		m_data = dataString;
		int delim1 = m_data.find_first_of(':');
		int delim2 = m_data.find_first_of(':', delim1+1);
		int delim3 = m_data.find_first_of(':', delim2+1);
		int delim4 = m_data.find_first_of(':', delim2+1);
		m_date = m_data.substr(0,delim1);
		m_version = m_data.substr(delim1+1, (delim2-delim1)-1);
		m_filepath = m_data.substr(delim2+1, (delim3-delim2)-1);
		m_comment = m_data.substr(delim3+1, (delim4-delim3)-1);
		m_event = m_data.substr(delim4+1, m_data.length());

	}

	HistoryItem(const char *date, const char *version, const char *filepath, const char *comment, const char *event) {
		m_date = date;
		m_version = version;
		m_filepath = filepath;
		m_comment = comment;
		m_event = event;
	}

	//bool add(const char *date, const char *version, int comment, const char *event);

	std::string &toString() {
		m_data = m_date + ":" + m_version + ":" + m_filepath +":" + m_comment +":" + m_event;
		return m_data;
	}

};
class EventList : public std::vector<HistoryItem> {};


bool History::m_isOpen = false;

History::History() {
	m_eventList = new EventList;

}

History::~History() {
	if (m_isOpen) {
		m_logfile.close();
	}
	delete m_eventList;
}

std::string History::m_filename = "";
History *History::m_this = NULL;
std::ofstream History::m_logfile;

void History::setPath(const char *path) {
	LogName logName;
	m_filename = logName.makeName(path, "hist", "log", 256);
}

History &History::getHistory() {

	if (m_this == NULL) {
		m_this = new History();
		m_logfile.open(m_filename.c_str(), std::ios::out | std::ios::app);
		if (m_logfile.is_open()) {
			m_isOpen = true;
		}
	}
	return *m_this;
}


/**
 * This function adds history to an image.
 */
bool History::add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he) {
	std::string filepathstr = filepath;
	std::string filename;
	std::string path;
	int slashpos = filepathstr.find_last_of("/");
	if (slashpos != -1) {
		filename = filepathstr.substr(slashpos+1, filepathstr.length() - slashpos);
		path = filepathstr.substr(0, slashpos);
	} else {
		filename = filepathstr;
	}
	//read(m_filename.c_str());
	ExifDateTime date;
	date.now();
	//CDate dateNow = CDate::timeNow();
	std::string dateStr = date.current();
	//std::string date =
	const char *event = he.getString();
	HistoryItem historyItem(dateStr.c_str(), filepath, version, comment, event);
	m_logfile << historyItem.toString() << '\n';

	return true;
}

bool History::read(const char *filepath) {
	char text[256];
	std::ifstream file(filepath);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		m_eventList->push_back(*(new HistoryItem(text)));
	}
	file.close();

	return true;
}





} /* namespace simplearchive */
