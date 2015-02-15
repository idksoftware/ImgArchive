/*
 * CShell.cpp
 *
 *  Created on: May 28, 2014
 *      Author: wzw7yn
 */
//#define _BSD_SOURCE
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>
#ifndef WIN32
#include <unistd.h>
#endif
//#include <sys/wait.h>
//#include <signal.h>


#ifdef LINUX
  # define sigmask(sig)   __sigmask(sig)
#endif


#include "ExternalShell.h"
namespace simplearchive {

ExternalShell::ExternalShell() {
	// TODO Auto-generated constructor stub

}

ExternalShell::~ExternalShell() {
	// TODO Auto-generated destructor stub
}
/*
 * Note use _popen and _pclose for windows
 */
#define MAX_LINE_LENGTH 255
bool ExternalShell::exec(const char *cmd) {
#ifdef WIN32
	std::string tmp = "\"";
	tmp += cmd;
	tmp += "\"";
	FILE* pipe = _popen(tmp.c_str(), "r");
#else
	FILE* pipe = popen(cmd, "r");
#endif
	if (!pipe) {
		return false;
	}
	char buffer[MAX_LINE_LENGTH + 1];
	m_output.clear();
	/*
	while (feof(pipe)) {
		if (fgets(buf, MAX_LINE_LENGTH, pipe) != NULL) {
			m_output += buf;
		}
	}
	*/
	if (pipe != NULL) {
		while (fgets( buffer, MAX_LINE_LENGTH, pipe ) != 0)
		{
			int length = strlen(buffer);
			if (length > 0  &&  buffer[length-1] == '\n')
			{
				buffer[--length] = '\0';
			}
			m_output += buffer;
		}
	}
#ifdef WIN32
	_pclose(pipe);
#else
	pclose(pipe);
#endif
	return true;
}

} /* namespace simplearchive */

