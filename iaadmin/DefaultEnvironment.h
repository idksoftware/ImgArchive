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
		static std::string m_homePath;
		static bool m_isHomePathSet;
		static bool m_isHomePathValid;
	public:
		DefaultEnvironment() = default;
		~DefaultEnvironment() = default;
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

		void print() {
			printf("Is configured %s\n", (m_configured)?"true":"False");
			printf("IsInAdminMode %s\n", (m_isInAdminMode)?"true" : "False");
			
			printf("Is Home path Set %s\n", (m_isHomePathSet)?"true" : "False");
			printf("Is Home path Valid %s\n", (m_isHomePathValid)?"true" : "False");
			printf("HomePath %s\n", m_homePath.c_str());
		}
	};

}