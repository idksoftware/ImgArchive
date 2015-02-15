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
// linux only #include <unistd.h>

//#include <sys/wait.h>
//#include <signal.h>


#ifdef LINUX
  # define sigmask(sig)   __sigmask(sig)
#endif


#include "CShell.h"
namespace simplearchive {

CShell::CShell() {
	// TODO Auto-generated constructor stub

}

CShell::~CShell() {
	// TODO Auto-generated destructor stub
}
/*
 * Note use _popen and _pclose for windows
 */
#define MAX_LINE_LENGTH 255
bool CShell::exec(const char *cmd) {
#ifdef _WIN32
	FILE* pipe = _popen(cmd, "r");
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
		if (fgets( buffer, MAX_LINE_LENGTH, pipe ))
		{
			int length = strlen(buffer);
			if (length > 0  &&  buffer[length-1] == '\n')
			{
				buffer[--length] = '\0';
			}
			m_output += buffer;
	    }
	  }
#ifdef _WIN32
	_pclose(pipe);
#else
	pclose(pipe);
#endif
	return true;
}

} /* namespace simplearchive */

