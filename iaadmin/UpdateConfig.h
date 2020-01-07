#pragma once

namespace simplearchive {

	class UpdateConfig
	{
		bool read(SIAARCConfig& imgaConfig);
	public:
		UpdateConfig() = default;
		~UpdateConfig() = default;
		
		bool update(const char* configOptionBlock, const char* configOption, const char* configValue);
	};

};

