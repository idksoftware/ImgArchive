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
#include <sstream>
#include <array>
#include <cstdio>
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

std::string returnedExecString;

std::string GetLastErrorStdStr();
int ExecuteExternalFile(std::string &csExeNameAndArgs);

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
		std::string cmdstr = cmd;
		bool ret = ExecuteExternalFile(cmdstr);
		m_output = returnedExecString;
		if (ret == false) {
			m_errorString = GetLastErrorStdStr();
		}
		return ret;

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
				size_t length = strlen(buffer);
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
#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <strsafe.h>
#endif
#include <string>
#include <stdio.h>



#ifdef _WIN32
int ExecuteExternalFile(std::string &csExeNameAndArgs)
{
	std::string csExecute;
	csExecute = csExeNameAndArgs;

	//std::string envVars = "hookScripts=test\0second=jam\0";

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
	if (CreateProcess(0, (LPSTR)csExecute.c_str(), 0, 0, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo) == false) {
#else
	if (CreateProcess(0, (LPSTR)csExecute.c_str(), 0, 0, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo) == false) {
#endif
		return false;
	}
	//now read the output pipe here.

	char buf[100 + 1];
	DWORD reDword;
	std::string m_csOutput, csTemp;
	BOOL res;
	std::stringstream streamBuf;
	LPDWORD lpBytesRead = 0;
	LPDWORD lpTotalBytesAvail = 0;
	LPDWORD lpBytesLeftThisMessage = 0;
	CloseHandle(wPipe);
	do
	{
		res = ::ReadFile(rPipe, buf, 100, &reDword, 0);	
		buf[reDword] = '\0';
		//printf("%s", buf);
		std::string temp(buf, reDword);
		streamBuf << temp;

	} while (res);
	m_csOutput = streamBuf.str();
	//printf("%s", m_csOutput.c_str());
	returnedExecString = m_csOutput;
	return true;
}

std::string GetLastErrorStdStr()
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + bufLen);

			LocalFree(lpMsgBuf);

			return result;
		}
  }
	return std::string();
}

#else

std::string GetLastErrorStdStr()
{
	return "Error";
}

int ExecuteExternalFile(std::string &csExeNameAndArgs) {

	returnedExecString.clear();
	auto pPipe = ::popen(csExeNameAndArgs.c_str(), "r");
	if(pPipe == nullptr)
	{
		throw std::runtime_error("Cannot open pipe");
	}

	std::array<char, 256> buffer;

	std::string result;

	while(not std::feof(pPipe))
	{
		auto bytes = std::fread(buffer.data(), 1, buffer.size(), pPipe);
		returnedExecString.append(buffer.data(), bytes);
	}

	auto rc = ::pclose(pPipe);



	return rc;
}

#endif




