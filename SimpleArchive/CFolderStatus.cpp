/*
 * CFolderStatus.cpp
 *
 *  Created on: May 21, 2014
 *      Author: wzw7yn
 */

#include "CFolderStatus.h"
#include <string>
#include <cstdlib>
#include <vector>

namespace simplearchive {
class CFolderStatusItem {
	typedef enum {
		CheckedIn, //
		CheckedOut,
		UnVersioned,	// Not versioned i.e. a changed image
						// with no versioned copy in the archive.
	} Status;
	Status m_status;
	std::string m_filename;

public:
	CFolderStatusItem();
	CFolderStatusItem(const char *dataString) {
		std::string m_data = dataString;
		int delim1 = m_data.find_first_of(':');
		int delim2 = m_data.find_first_of(delim1, ':');
		std::string crcStr = m_data.substr(0,delim1);
		int m_crc = strtol(crcStr.c_str(),NULL,16);
		std::string name = m_data.substr(delim1, delim2);
		std::string md5 = m_data.substr(delim2, m_data.length());
	}
};

class CFolderStatusContainer : public std::vector<CFolderStatusItem> {

public:

};


CFolderStatus::CFolderStatus() {
	// TODO Auto-generated constructor stub

}

CFolderStatus::~CFolderStatus() {
	// TODO Auto-generated destructor stub
}

bool update() {
	return true;
}

} /* namespace simplearchive */
