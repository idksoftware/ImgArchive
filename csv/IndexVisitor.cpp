/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

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
#include "IndexVisitor.h"
#include "SAUtils.h"
#include "CheckoutStatus.h"
#include "pathcontroller.h"
#include "DBDefines.h"
#include "MetaType.h"
#include "AddressScope.h"
#include "CLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	std::string IndexVisitor::m_master;
	std::string IndexVisitor::m_workspace;
	std::string IndexVisitor::m_primaryIndex;

	/*
	ReporterItem::ReporterItem(ReporterItem::Status status, std::string & message) : m_message(message), m_status(status)
	{

		try {
			CLogger &logger = CLogger::getLogger();
			std::string msg = statusString();
			msg += ": ";
			msg += message;
			logger.log(LOG_OK, CLogger::Level::STATUS, msg.c_str());
		}
		catch (std::exception &e) {
			throw e;
		}
	}
	*/
	/*
	void IndexActionReporter::add(ReporterItem::Status status, const char * fmt, ...)
	{
		
		
		char message[1024];
		try {

			va_list args;
			va_start(args, fmt);
#ifdef _WIN32
			vsprintf_s(message, fmt, args);
#else
			vsprintf(message, format, args);
#endif
			va_end(args);
		}
		catch (std::exception e) {
			printf("logger crashed parsing message");
			exit(-1);
		}
		std::string finalStr = message;
		push_back(ReporterItem(status, finalStr));
		
	}
	*/

	IndexVisitor::IndexVisitor(std::shared_ptr<IndexAction> indexAction) {
		m_indexAction = indexAction;
		m_addressScope = std::make_unique<AddressScope>();
		indexAction->init(m_master.c_str(), m_workspace.c_str(), m_primaryIndex.c_str());
	}

	IndexVisitor::~IndexVisitor() {

	}

	bool IndexVisitor::Init(const char *master, const char *workspace, const char *primaryIndex) {
		m_master = master;
		m_workspace = workspace;
		m_primaryIndex = primaryIndex;
		
		return true;
	}


	bool IndexVisitor::setScope(const char *scope) {
		
		std::string scopeStr = scope;

		if (scopeStr.empty()) {
			m_addressScope->scopeAll();
		}
		else if (scopeStr.compare("all") == 0) {
			m_addressScope->scopeAll();
		}
		else if (!m_addressScope->scope(scope)) {
			return false;
		}
		return true;
	}

	bool IndexVisitor::process() {
		if (!IndexVisitor::process(m_primaryIndex.c_str())) {
			return false;
		}
		return true;
	}

	bool IndexVisitor::process(const char *rootFolder) {
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
			catch (std::exception /*e*/) {
				return false;
			}

			yearMaster += '/';
			yearMaster += year;

			if (!m_addressScope->isInScope(year.c_str())) {
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
				if (!m_addressScope->isInScope(dateStr.c_str())) {
					continue;
				}
				PathController pathController(dayfolder.c_str());
				m_indexAction->onDayFolder(dayfolder.c_str());
			
				std::string filenameStr = yearMaster;
				filenameStr += '/';
				filenameStr += dayfolder;
				m_indexAction->m_currentPartition = std::make_shared<CheckoutPartition>();
				m_indexAction->onDayFolder(dayfolder.c_str());
				if (m_indexAction->m_currentPartition->read(filenameStr.c_str()) == false) {
					return false;
				}
				
				for (auto i = m_indexAction->m_currentPartition->begin(); i != m_indexAction->m_currentPartition->end(); i++) {
					std::shared_ptr<MTRow> row = *i;
					m_indexAction->m_currentRow = std::static_pointer_cast<CheckoutRow>(row);
					const char *fileName = m_indexAction->m_currentRow->getFileName();
					if (!m_addressScope->isImageInScope(fileName)) {
						continue;
					}
					if (m_indexAction->onImage() == false) {
						return false;
					}
				}
				
				//checkoutPartition.write(filenameStr.c_str());
				m_indexAction->onDayEnd();
			}
			m_indexAction->onYearEnd();

		}
		m_indexAction->onEnd();
		return true;
	}

	bool onDayFolder(const char *name)
	{
		return true;
	};
	
	bool StatusAction::onEnd()
	{
		if (!log->write(LogDocument::FormatType::Human)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	};

	bool StatusAction::onImage()
	{
		std::string s = m_currentRow->toString();
		log->push_back(s);
		return true;
	}

	bool StatusAction::onStart()
	{
		log = std::make_shared<CheckoutStatusLog>();
		return true;
	};

	
	/*
	const char * ReporterItem::statusString()
	{
		switch (m_status) {
		case ReporterItem::Status::Completed: return	"Completed ";
		case ReporterItem::Status::Warning: return		"Warning   ";
		case ReporterItem::Status::Error: return		"Error     ";
		case ReporterItem::Status::Fatal: return		"Fatal     ";
		}
		return											"Unknown   ";
	}
	*/

	

} /* namespace simplearchive */
