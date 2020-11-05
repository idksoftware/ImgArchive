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

		static bool locations(const char* path);

	public:

		DefaultEnvironment() = default;
		~DefaultEnvironment() = default;
		
		static bool setAllUserDefaultLocations();
		static bool setLocalDefaultLocations();
		static bool setDefaultLocations();

		static bool setNewInstallAllUserDefaultLocations();
		static bool setNewInstallLocalDefaultLocations();
		static bool setNewInstallDefaultLocations();

		
	
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

}