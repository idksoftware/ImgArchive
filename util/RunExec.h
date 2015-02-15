#pragma once
#include <string>

class RunExec
{
	std::string m_workingFolder;
	std::string m_execPath;

public:
	RunExec();
	RunExec(const char *workingFolder);
	~RunExec();
	void execPath(const char *path) {
		m_execPath = path;
	}
	bool exec(const char *cmdline);
};
