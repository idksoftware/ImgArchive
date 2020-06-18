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

//#define _BSD_SOURCE
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>
// linux only #include <unistd.h>

//#include <sys/wait.h>
//#include <signal.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


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
			size_t length = strlen(buffer);
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

