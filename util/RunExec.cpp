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

#include "RunExec.h"

#include "stdafx.h"
#include <stdio.h> 
#include <windows.h> 

#define BUFSIZE 4096 

static HANDLE hChildStdinRd, hChildStdinWr, hChildStdinWrDup,
hChildStdoutRd, hChildStdoutWr, hChildStdoutRdDup,
hInputFile, hSaveStdin, hSaveStdout;

std::string m_errorString;

static bool CreateChildProcess(char *cmdline);
static void WriteToPipe(void);
static void ReadFromPipe(void);
static void ErrMsg(LPTSTR, BOOL);

RunExec::RunExec()
{

}

RunExec::RunExec(const char *workingFolder)
{
}


RunExec::~RunExec()
{
}

bool RunExec::exec(const char *cmdline)
{
	SECURITY_ATTRIBUTES saAttr;
	BOOL fSuccess;

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// The steps for redirecting child process's STDOUT: 
	//     1. Save current STDOUT, to be restored later. 
	//     2. Create anonymous pipe to be STDOUT for child process. 
	//     3. Set STDOUT of the parent process to be write handle to 
	//        the pipe, so it is inherited by the child process. 
	//     4. Create a noninheritable duplicate of the read handle and
	//        close the inheritable read handle. 

	// Save the handle to the current STDOUT. 

	hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Create a pipe for the child process's STDOUT. 

	if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
		m_errorString = "Stdout pipe creation failed\n";
		return false;
	}



	// Set a write handle to the pipe to be STDOUT. 

	if (!SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr)) {
		m_errorString = "Redirecting STDOUT failed";
		return false;
	}
	// Create noninheritable read handle and close the inheritable read 
	// handle. 

	fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
		GetCurrentProcess(), &hChildStdoutRdDup, 0,
		FALSE,
		DUPLICATE_SAME_ACCESS);
	if (!fSuccess) {
		m_errorString = "DuplicateHandle failed";
		return false;
	}
	CloseHandle(hChildStdoutRd);

	// The steps for redirecting child process's STDIN: 
	//     1.  Save current STDIN, to be restored later. 
	//     2.  Create anonymous pipe to be STDIN for child process. 
	//     3.  Set STDIN of the parent to be the read handle to the 
	//         pipe, so it is inherited by the child process. 
	//     4.  Create a noninheritable duplicate of the write handle, 
	//         and close the inheritable write handle. 

	// Save the handle to the current STDIN. 

	hSaveStdin = GetStdHandle(STD_INPUT_HANDLE);

	// Create a pipe for the child process's STDIN. 

	if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) {
		m_errorString = "Stdin pipe creation failed\n";
		return false;
	}

	// Set a read handle to the pipe to be STDIN. 

	if (!SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd)) {
		m_errorString = "Redirecting Stdin failed";
		return false;
	}

	// Duplicate the write handle to the pipe so it is not inherited. 

	fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr,
		GetCurrentProcess(), &hChildStdinWrDup, 0,
		FALSE,                  // not inherited 
		DUPLICATE_SAME_ACCESS);
	if (!fSuccess) {
		m_errorString = "DuplicateHandle failed";
		return false;
	}

	CloseHandle(hChildStdinWr);

	// Now create the child process. 
	std::string fullExec;
	if (m_execPath.empty() == false) {
		fullExec = m_execPath;
		fullExec += '/';
		fullExec += cmdline;

	}
	else {
		fullExec = cmdline;
	}

	if (!CreateChildProcess((char*)fullExec.c_str())) {
		m_errorString = "Create process failed";
		return false;
	}

	// After process creation, restore the saved STDIN and STDOUT. 

	if (!SetStdHandle(STD_INPUT_HANDLE, hSaveStdin)) {
		m_errorString = "Re-redirecting Stdin failed\n";
		return false;
	}

	if (!SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout)) {
		m_errorString = "Re-redirecting Stdout failed";
		return false;
	}

	// Get a handle to the parent's input file. 

	if (0) {
		hInputFile = CreateFile("text.txt", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	}
	else {
		hInputFile = hSaveStdin;
	}

	if (hInputFile == INVALID_HANDLE_VALUE) {
		m_errorString = "no input file";
		return false;
	}

	// Write to pipe that is the standard input for a child process. 

	//	WriteToPipe();

	// Read from pipe that is the standard output for child process. 

	ReadFromPipe();
	m_errorString = "Complete";
	return true;
}

bool CreateChildProcess(char *cmdline)
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;

	// Set up members of STARTUPINFO structure. 

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);

	// Create the child process. 

	return CreateProcess(NULL,
		cmdline,       // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 
}

void WriteToPipe(void)
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];

	// Read from a file and write its contents to a pipe. 

	for (;;)
	{
		if (!ReadFile(hInputFile, chBuf, BUFSIZE, &dwRead, NULL) ||
			dwRead == 0) break;
		if (!WriteFile(hChildStdinWrDup, chBuf, dwRead,
			&dwWritten, NULL)) break;
	}

	// Close the pipe handle so the child process stops reading. 

	if (!CloseHandle(hChildStdinWrDup))
		m_errorString = "Close pipe failed";
}

void ReadFromPipe(void)
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Close the write end of the pipe before reading from the 
	// read end of the pipe. 

	if (!CloseHandle(hChildStdoutWr))
		m_errorString = "Closing handle failed";

	// Read output from the child process, and write to parent's STDOUT. 

	for (;;)
	{
		if (!ReadFile(hChildStdoutRdDup, chBuf, BUFSIZE, &dwRead,
			NULL) || dwRead == 0) break;
		if (!WriteFile(hSaveStdout, chBuf, dwRead, &dwWritten, NULL))
			break;
	}
}


/*
// The code for the child process.

#include <windows.h>
#define BUFSIZE 4096

void main(void)
{
CHAR chBuf[BUFSIZE];
DWORD dwRead, dwWritten;
HANDLE hStdin, hStdout;
BOOL fSuccess;

hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
hStdin = GetStdHandle(STD_INPUT_HANDLE);
if ((hStdout == INVALID_HANDLE_VALUE) ||
(hStdin == INVALID_HANDLE_VALUE))
ExitProcess(1);

for (;;)
{
// Read from standard input.
fSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL);
if (!fSuccess || dwRead == 0)
break;

// Write to standard output.
fSuccess = WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL);
if (!fSuccess)
break;
}
}
*/



