#pragma once

#include <string>

namespace simplearchive {
	/*
	This class sets the default Environment up so the Environment is in
	a known state.
	*/

	enum class HomePathType;

	class DefaultEnvironment
	{
		static bool m_configured;
		static bool m_isInAdminMode;
		static bool m_isHomePathSet;
		static bool m_isHomePathValid;

		static bool locations(const char* path);

	public:

		DefaultEnvironment() = default;
		~DefaultEnvironment() = default;
		
		static bool setAllUserDefaultLocations();
		static bool setLocalDefaultLocations();
		static bool setDefaultLocations();

		

		
	
		static bool init();
		static bool isConfigured() {
			return m_configured;
		}
		static bool isInAdminMode() {
			return m_isInAdminMode;
		}
	
		

		void print() {
			printf("Is configured %s\n", (m_configured)?"true":"False");
			printf("IsInAdminMode %s\n", (m_isInAdminMode)?"true" : "False");
			
			printf("Is Home path Set %s\n", (m_isHomePathSet)?"true" : "False");
			printf("Is Home path Valid %s\n", (m_isHomePathValid)?"true" : "False");
			//printf("HomePath %s\n", m_homePath.c_str());
		}
	};

	class NewInstallOptions {
		std::string m_homePath;
		std::string m_masterPath;
		std::string m_derivativePath;
		std::string m_workspacePath;
		std::string m_picturePath;
		std::string m_wwwPath;
	public:
		NewInstallOptions() = default;
		~NewInstallOptions() = default;

	std::string getDerivativePath() const;
	void setDerivativePath(std::string mDerivativePath);
	std::string getHomePath() const;
	void setHomePath(std::string mHomePath);
	std::string getMasterPath() const;
	void setMasterPath(std::string mMasterPath);
	std::string getPicturePath() const;
	void setPicturePath(std::string mPicturePath);
	std::string getWorkspacePath() const;
	void setWorkspacePath(std::string mWorkspacePath);
	std::string getWwwPath() const;
	void setWwwPath(std::string mWwwPath);
};

	class NewInstallDefaultLocations {

	public:
		enum class HomePathType {
			Default,	// Default value
			LocalEnv,	// Local Environment set
			SystemEnv,	// System Environment set
			UserOnly,	// user only archive
			AllUsers,	// all users archive
			Unknown
		};

	private:

		static bool m_configured;
		static bool m_isInAdminMode;
		static bool m_isHomePathSet;
		static bool m_isHomePathValid;
		static std::string m_imgArchiveHome;
		static HomePathType m_type;
		static bool m_found;

		NewInstallOptions m_newInstallOptions;

	public:

		NewInstallDefaultLocations() = default;
		~NewInstallDefaultLocations() = default;

		static bool init();
		static bool setAllUserDefaultLocations();
		static bool setLocalDefaultLocations();
		static bool setDefaultLocations();

		NewInstallOptions& getOptions();

		void setDerivativePath(std::string mDerivativePath);
		void setHomePath(std::string mHomePath);
		void setMasterPath(std::string mMasterPath);
		void setPicturePath(std::string mPicturePath);
		void setWorkspacePath(std::string mWorkspacePath);
		void setWwwPath(std::string mWwwPath);
	};
}
