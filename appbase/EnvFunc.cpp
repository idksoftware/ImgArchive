// regedit.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#include <windows.h>
#endif
#include <string>
#include <stdio.h>
#include "EnvFunc.h"

#ifdef _WIN32
bool SetEnv(const char *szIAHome, bool all)
{
    HKEY   hkey;
    DWORD  dwDisposition;
    DWORD dwType, dwSize;
	LONG result;
	if (all) {
		char *regPath = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regPath,
										0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
	else {
		char *regPath = "Environment";
		result = RegCreateKeyEx(HKEY_CURRENT_USER, regPath,
										0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
   
    if(result == ERROR_SUCCESS)
    {
        dwType = REG_SZ;
        dwSize = strlen(szIAHome)+1;
        LONG setResult = RegSetValueEx(hkey, TEXT("IMGARCHIVE_HOME"), 0, dwType,
        (PBYTE)szIAHome, dwSize);
        RegCloseKey(hkey);
        return setResult == ERROR_SUCCESS;
    }
    else
    {
        return false;
    }
}

#ifdef XXXXXX 
bool GetEnv(const char *szIAHome, std::string &resultStr, bool all)
{
	HKEY   hkey;
	DWORD  dwDisposition;
	DWORD dwType, dwSize;
	LONG result;
	/*
	if (all) {
		char *regPath = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regPath,
			0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
	else {
		char *regPath = "Environment";
		result = RegCreateKeyEx(HKEY_CURRENT_USER, regPath,
			0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
	*/
	char *regPath = "Environment";
	char path[MAX_PATH + 1];
	DWORD size = MAX_PATH;
	result = RegQueryValueEx(HKEY_CURRENT_USER, regPath, nullptr, nullptr, (LPBYTE)path, &size);
	resultStr = path;
	return true;
}
#endif

bool GetEnv(std::string& value, bool all) {
	HKEY hKey = 0;
	char buf[MAX_PATH];
	DWORD dwType = 0;
	DWORD dwBufSize = MAX_PATH;
	if (all) {
		const char* subkey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) {
			return false;
		}
	}
	else {
		const char* subkey = "Environment";
		if (RegOpenKeyEx(HKEY_CURRENT_USER, subkey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) {
			return false;
		}
	}
	if (RegQueryValueEx(hKey, "IAHOME", NULL, NULL, (BYTE*)buf, &dwBufSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return false;
	}
	value = buf;
	RegCloseKey(hKey);
	return true;
}

#else
bool SetEnv(const char *szIAHome)
{
	return false;
}
#endif



