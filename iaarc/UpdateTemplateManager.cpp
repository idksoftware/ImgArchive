#include <string>
#include "UpdateTemplateManager.h"
#include "UpdateTemplateFile.h"

namespace simplearchive {

	bool UpdateTemplateManager::process(bool repo, bool templateFile, const char* option) {


		if (m_templatePath.empty()) {

			return false;
		}
		std::string templatefilename;
		if (repo) {
			// Master
			if (templateFile) {
				templatefilename = "master.tpl";
			}
			else {
				templatefilename = "master_base.tpl";
			}
		}
		else {
			// Derivative
			if (templateFile) {
				templatefilename = "derivative.tpl";
			}
			else {
				templatefilename = "derivative_base.tpl";
			}
		}

		if (processArgs(option) == false) {
			return false;
		}

		UpdateTemplateFile updateTemplateFile;

		updateTemplateFile.update(m_templatePath.c_str(), templatefilename.c_str(), m_option.c_str(), m_value.c_str());
		return true;
	}

	bool UpdateTemplateManager::processArgs(const char* ov) {
		std::string optionValueString = ov;
		size_t pos = optionValueString.find_first_of('=');
		if (pos == std::string::npos) {
			return false;
		}
		m_option = optionValueString.substr(0, pos);
		m_value = optionValueString.substr(pos + 1, optionValueString.length() - 1);

		return true;
	}

}