
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
//#include <dirent.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
//#include <unistd.h>
#ifdef WINDOWS
#include <sysstat.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdarg>
#include "CIDKFileFind.h"

#include "SAUtils.h"

#include "pathcontroller.h"
#include "DBDefines.h"
#include "MetaType.h"
#include "AddressScope.h"
#include "CLogger.h"
#include "CSVIndexVisitor.h"
#include "CSVIndexAction.h"

namespace simplearchive {

	CSVIndexVisitor::CSVIndexVisitor(std::shared_ptr<CSVIndexAction> indexAction, const char* indexPath) {
		m_indexAction = indexAction;
		m_indexAction->setAddressScope(m_addressScope);
		m_indexPath = indexPath;
	}

	CSVIndexVisitor::~CSVIndexVisitor() {

	}




	bool CSVIndexVisitor::setScope(const char* scope) {

		std::string scopeStr;
		if (scope != nullptr) {
			std::string scopeStr = scope;
		}

		if (scopeStr.empty()) {
			m_addressScope.scopeAll();
		}
		else if (scopeStr.compare("all") == 0) {
			m_addressScope.scopeAll();
		}
		else if (!m_addressScope.scope(scope)) {
			return false;
		}
		return true;
	}

	bool CSVIndexVisitor::process() {
		if (!CSVIndexVisitor::process(m_indexPath.c_str())) {
			return false;
		}
		return true;
	}

	bool CSVIndexVisitor::process(const char* rootFolder) {
		std::string path = rootFolder;

		m_indexAction->onStart();
		// read years in Master folder
		FileList_Ptr filelist = SAUtils::getFiles_(path.c_str());
		for (auto i = filelist->begin(); i != filelist->end(); i++) {
			std::string year = *i;
			char c = (year)[0];
			if (c == '.') {
				continue;
			}

			std::string yearMaster = path;
			try {
				m_indexAction->onYearFolder(year.c_str());
			}
			catch (std::exception) { //e*) {
				return false;
			}

			yearMaster += '/';
			yearMaster += year;

			if (!m_addressScope.isInScope(year.c_str())) {
				continue;
			}


			FileList_Ptr filelist = SAUtils::getFiles_(yearMaster.c_str());
			for (auto i = filelist->begin(); i != filelist->end(); i++) {
				std::string dayfolder = *i;
				char c = (dayfolder)[0];
				if (c == '.') {
					continue;
				}
				std::string dateStr = dayfolder.substr(0, 10);
				if (!m_addressScope.isInScope(dateStr.c_str())) {
					continue;
				}

				PathController pathController(dayfolder.c_str(), false);

				std::string filenameStr = yearMaster;
				filenameStr += '/';
				filenameStr += dayfolder;

				m_indexAction->onDayFolder(filenameStr.c_str());
				//std::string dayFolderMaster = yearMaster;
				//dayFolderMaster += '/';
				//dayFolderMaster += dayfolder;

				FileList_Ptr filelist = SAUtils::getFiles_(filenameStr.c_str());
				for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
					std::string imageFile = *i;
					char c = (imageFile)[0];
					if (c == '.') {
						continue;
					}
					//printf("\t\tImage %s: \n", imageFile->c_str());
					std::string itemPath = filenameStr;
					itemPath += '/';
					itemPath += imageFile;
					m_indexAction->onImage(itemPath.c_str());
				}
				m_indexAction->onDayEnd();
			}
			m_indexAction->onYearEnd();

		}
		m_indexAction->onEnd();
		return true;
	}
};