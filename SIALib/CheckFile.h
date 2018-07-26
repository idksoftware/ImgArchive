#pragma once

namespace simplearchive {

	class CheckFile {
		static std::string m_workspacePath;
		CheckFile() {};
		static std::string getFullAddress(const char *address);
	public:
		static bool initalise(const char *workspacePath);
		static bool CheckOut(const char *address);
		static bool CheckIn(const char *addresss);
	};
}; // simplearchive