#pragma once
#include <string>
#include <vector>
#include "PageMakerBase.h"
#include "SAUtils.h"


namespace simplearchive {
	
	class ImagePageMaker
	{
		std::string m_templatePath;
		std::string m_htmlPath;
		std::string m_rootPath;
		std::string m_archiveRootPath;
		bool process(const char *fileIn, const char *fileOut);
	public:
		ImagePageMaker();
		~ImagePageMaker();
		bool setTemplatePath(const char *templatePath, const char* rootPath) {
			m_templatePath = templatePath;
			if (SAUtils::DirExists(templatePath) == false) {
				return false;
			}
			m_rootPath = rootPath;
			m_htmlPath = rootPath;
			m_htmlPath += '/';
			m_htmlPath += "images";
			return true;
			
		}
		void setArchiveRootPath(const char* archiveRootPath) {
			m_archiveRootPath = archiveRootPath;
		}
		bool process(DaysProcessed &daysProcessed);
	};


	
} // namespace simplearchive
