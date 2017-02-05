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
#ifndef _WIN32
#include <unistd.h>
#endif
//#include <sys/wait.h>
//#include <signal.h>


#ifdef LINUX
  # define sigmask(sig)   __sigmask(sig)
#endif


#include "ExternalShell.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

std::string ExecuteExternalFile(std::string csExeNameAndArgs);

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
		m_output = ExecuteExternalFile(cmd);
		return true;

#ifdef _WIN32
		//m_output = ExecuteExternalFile(cmd);
		FILE* pipe = _popen(cmd, "rt");
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
			while (fgets(buffer, MAX_LINE_LENGTH, pipe) != 0)
			{
				int length = strlen(buffer);
				if (length > 0 && buffer[length - 1] == '\n')
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
	bool ExternalShell::executeBatch(const char *BatchFile) {

		std::string cmd(BatchFile);
		cmd += " dsc_3248";
		cmd += " nef";
		std::string expandCmd = std::string("\"") + cmd + std::string("\"");
		system(expandCmd.c_str());
		return true;
	}
} /* namespace simplearchive */

#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <strsafe.h>
#include <string>
#include <stdio.h>
#include <Windows.h>

std::string ExecuteExternalFile(std::string csExeNameAndArgs)
{
	std::string csExecute;
	csExecute = csExeNameAndArgs;

	std::string envVars = "hookScripts=test\0second=jam\0";

	SECURITY_ATTRIBUTES secattr;
	ZeroMemory(&secattr, sizeof(secattr));
	secattr.nLength = sizeof(secattr);
	secattr.bInheritHandle = TRUE;

	HANDLE rPipe, wPipe;

	//Create pipes to write and read data

	CreatePipe(&rPipe, &wPipe, &secattr, 0);
	//

	STARTUPINFO sInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&pInfo, sizeof(pInfo));
	sInfo.cb = sizeof(sInfo);
	sInfo.dwFlags = STARTF_USESTDHANDLES;
	sInfo.hStdInput = NULL;
	sInfo.hStdOutput = wPipe;
	sInfo.hStdError = wPipe;

	//Create the process here.
#ifdef _WIN64
	CreateProcess(0, (LPWSTR)csExecute.c_str(), 0, 0, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo);
#else
	CreateProcess(0, (LPSTR)csExecute.c_str(), 0, 0, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo);
#endif
	CloseHandle(wPipe);

	//now read the output pipe here.

	char buf[100];
	DWORD reDword;
	std::string m_csOutput, csTemp;
	BOOL res;
	do
	{
		res = ::ReadFile(rPipe, buf, 100, &reDword, 0);
		std::string *temp = new std::string(buf, reDword);
		csTemp = *temp;
		m_csOutput += csTemp;
	} while (res);
	return m_csOutput;
}





