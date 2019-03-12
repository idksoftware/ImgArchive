#pragma once

namespace simplearchive {
	/*
	class CheckWorkspaceFile {
		static std::string m_workspacePath;
		CheckWorkspaceFile() {};
		static std::string getFullAddress(const char *address);
	public:
		static bool initalise(const char *workspacePath);
		static bool CheckOut(const char *address);
		static bool CheckIn(const char *addresss);
	};

	class CheckMasterFile {
		static std::string m_masterPath;
		CheckMasterFile() {};
		static std::string getFullAddress(const char *address);
	public:
		static bool initalise(const char *workspacePath);
		static bool CheckOut(const char *address);
		static bool CheckIn(const char *addresss);
	};
	*/
	class CDCheckInOutManager {
	public:
		enum class DB {
			Workspace,
			Master,
			Derivative
		};
	private:
		static std::string m_masterPath;
		static std::string m_workspacePath;
		static std::string m_derivativePath;
		
		static std::string getFullAddress(DB db, const char *address);
		CDCheckInOutManager() = default;
	public:
		
		virtual ~CDCheckInOutManager() = default;
		static void initalise(const char *workspacePath, const char *masterPath, const char *derivativePath) {
			m_workspacePath = workspacePath;
			m_masterPath = masterPath;
			m_derivativePath = derivativePath;
		}
		bool CheckOut(DB db, const char *address);
		bool CheckIn(DB db, const char *address);

		static CDCheckInOutManager &get();
	};

}; // simplearchive