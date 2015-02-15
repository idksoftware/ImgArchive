/*
 * CShell.h
 *
 *  Created on: May 28, 2014
 *      Author: wzw7yn
 */

#ifndef CSHELL_H_
#define CSHELL_H_
#include <stdio.h>

namespace simplearchive {

class CShell {
	std::string m_output;
public:
	CShell();
	virtual ~CShell();
	bool exec(const char *cmd);

	const std::string& getOutput() const {
		return m_output;
	}
};

} /* namespace simplearchive */
#endif /* CSHELL_H_ */
