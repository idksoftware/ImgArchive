#pragma once
class UpdateTemplateFile
{
};

#pragma once
#include <string>

namespace simplearchive {

	class ConfigReader;

	class UpdateTemplateFile
	{
		bool read(ConfigReader& config);
		std::string m_file;
		std::string m_rootPath;
	public:
		UpdateTemplateFile() = default;
		~UpdateTemplateFile() = default;

		bool update(const char* path, const char* file, const char* option, const char* value);
	};

};