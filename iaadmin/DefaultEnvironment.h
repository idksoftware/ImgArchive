#pragma once

#include <string>

namespace simplearchive {
	/*
	This class sets the default Environment up so the Environment is in
	a known state.
	*/

	class DefaultEnvironment
	{
		static bool m_configured;
		static bool m_isInAdminMode;
		static bool m_isHomePathSet;
		static bool m_isHomePathValid;

		static std::string m_homePath;
		static std::string m_picturesPath;
		static std::string m_archivePath;
		static std::string m_workspacePath;
		static std::string m_masterPath;
		static std::string m_derivativePath;
		static std::string m_cataloguePath;
		static std::string m_wwwCataloguePath;
		static std::string m_historyPath;

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
	
		static std::string& getHomePath() {
			return m_homePath;
		}
		static bool isHomePathSet() {
			return m_isHomePathSet;
		}
		static bool isHomePathValid() {
			return m_isHomePathValid;
		}

		static std::string  getArchivePath() {
			return m_archivePath;
		}

		static std::string  getWorkspacePath() {
			return  m_workspacePath;
		}

		static std::string  getMasterPath() {
			return  m_masterPath;
		}

		static std::string  getDerivativePath() {
			return  m_derivativePath;
		}

		static std::string  m_getCataloguePath() {
			return  m_cataloguePath;
		}

		static std::string m_getWwwCataloguePath() {
			return m_wwwCataloguePath;
		}

		static std::string  getHistoryPath() {
			return  m_historyPath;
		}


		void print() {
			printf("Is configured %s\n", (m_configured)?"true":"False");
			printf("IsInAdminMode %s\n", (m_isInAdminMode)?"true" : "False");
			
			printf("Is Home path Set %s\n", (m_isHomePathSet)?"true" : "False");
			printf("Is Home path Valid %s\n", (m_isHomePathValid)?"true" : "False");
			printf("HomePath %s\n", m_homePath.c_str());
		}
	};

}