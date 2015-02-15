/*
 * CShell.h
 *
 *  Created on: May 28, 2014
 *      Author: wzw7yn
 */

#ifndef EXTERNALSHELL_H_
#define EXTERNALSHELL_H_
#include <stdio.h>

namespace simplearchive {

class ExternalShell {
	std::string m_output;
public:
	ExternalShell();
	virtual ~ExternalShell();
	bool exec(const char *cmd);

	const std::string& getOutput() const {
		return m_output;
	}
};

} /* namespace simplearchive */
#endif /* EXTERNALSHELL_H_ */
