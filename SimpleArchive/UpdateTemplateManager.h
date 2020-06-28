#pragma once

namespace simplearchive {

	class UpdateTemplateManager
	{
		std::string m_option;
		std::string m_value;
		std::string m_templatePath;
		bool processArgs(const char* ov);
	public:
		UpdateTemplateManager(const char* templatePath) {
			m_templatePath = templatePath;
		}

		bool process(bool repo, bool templateFile, const char* option);
	};

};