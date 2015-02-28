/*
 * CFolderStatus.h
 *
 *  Created on: May 21, 2014
 *      Author: wzw7yn
 */

#ifndef CFOLDERSTATUS_H_
#define CFOLDERSTATUS_H_

#include <string>

namespace simplearchive {

class CFolderStatus {
	std::string m_pathToFolder;
public:
	CFolderStatus();
	virtual ~CFolderStatus();
	const std::string& getPathToFolder() const {
		return m_pathToFolder;
	}
	bool update() {

	}
};

} /* namespace simplearchive */
#endif /* CFOLDERSTATUS_H_ */
