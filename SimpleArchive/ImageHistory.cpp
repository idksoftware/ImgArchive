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
#include "ImageHistory.h"
#include "HistoryEvent.h"

namespace simplearchive {
class ImageHistoryItem {
	std::string m_comment;
	std::string m_date;
	std::string m_event;
	std::string m_data;
	std::string m_version;


public:
	ImageHistoryItem();
	/*
	 * 14.06.12.12.16.12:56:first checkin:checkin
	 */
	ImageHistoryItem(const char *dataString) {
		m_data = dataString;
		int delim1 = m_data.find_first_of(':');
		int delim2 = m_data.find_first_of(':', delim1+1);
		int delim3 = m_data.find_first_of(':', delim2+1);
		m_date = m_data.substr(0,delim1);
		m_version = m_data.substr(delim1+1, (delim2-delim1)-1);
		m_comment = m_data.substr(delim2+1, (delim3-delim2)-1);
		m_event = m_data.substr(delim3+1, m_data.length());

	}

	ImageHistoryItem(const char *date, const char *version, const char *comment, const char *event) {
		m_date = date;
		m_version = version;
		m_comment = comment;
		m_event = event;
	}

	//bool add(const char *date, const char *version, int comment, const char *event);

	std::string &toString() {
		m_data = m_date + ":" + m_version + ":" + m_comment +":" + m_event;
		return m_data;
	}

};
class EventList : public std::vector<ImageHistoryItem> {};



ImageHistory::ImageHistory(const char *datapath) {
	m_eventList = new EventList;
	m_datapath = datapath;
}

ImageHistory::~ImageHistory() {
	m_eventList->clear();
	delete m_eventList;
}
/**
 * This function adds history to an image.
 */
/*
bool ImageHistory::add(const char *filepath, const char *version, const char *comment, ImageHistory::HistoryEvent he) {
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
	read(m_datapath.c_str());
	std::string date = "14.09.01.12.45.19";
	const char *event = this->getEventString(he);
	ImageHistoryItem *historyItem = new ImageHistoryItem(date.c_str(), version, comment, event);
	m_eventList->insert(m_eventList->begin(), *historyItem);
	write(m_datapath.c_str());
	return true;
}
*/
bool ImageHistory::add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he) {
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
	read(m_datapath.c_str());
	std::string date = "14.09.01.12.45.19";
	const char *event = he.getString();
	ImageHistoryItem *historyItem = new ImageHistoryItem(date.c_str(), version, comment, event);
	m_eventList->insert(m_eventList->begin(), *historyItem);
	write(m_datapath.c_str());
	return true;
}

bool ImageHistory::read(const char *filepath) {
	char text[256];
	std::ifstream file(filepath);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		m_eventList->push_back(*(new ImageHistoryItem(text)));
	}
	file.close();

	return true;
}
bool ImageHistory::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<ImageHistoryItem>::iterator i = m_eventList->begin(); i != m_eventList->end(); i++) {
		ImageHistoryItem item = *i;
		std::string &line = item.toString();
		//printf("%s\n", line.c_str());
		file << line.c_str() << '\n';

	}
	file.close();
	return true;
}

/*
const char *ImageHistory::getEventString(HistoryEvent he) {
	switch (he) {
	case IMPORT:
		return "added";
	case CHECKOUT:
		return "checkout";
	case CHECKIN:
		return "checkin";
	case EXPORT:
		return "export";
	case ERROR:
		return "error";
	case UNCHECKOUT:
		return "uncheckout";
	}
	return "error";
}
*/

/*
HistoryEvent ImageHistory::getEvent(const char *he) {
	std::string eventstr = he;
	if (eventstr.compare("add") == 0) {
		return IMPORT;
	} else if(eventstr.compare("checkout") == 0) {
		return CHECKOUT;
	} else if(eventstr.compare("checkin") == 0) {
		return CHECKIN;
	} else if(eventstr.compare("export") == 0) {
		return EXPORT;
	}
	return ERROR;
}
*/



} /* namespace simplearchive */
