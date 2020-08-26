#pragma once
#include <string>

namespace simplearchive {

	class AppConfigBase;

	class UpdateConfig
	{
		bool read(AppConfigBase& imgaConfig);
		std::string m_configfile;
	public:
		UpdateConfig() = default;
		~UpdateConfig() = default;
		
		bool update(const char* configOptionBlock, const char* configOption, const char* configValue);
	};

};

