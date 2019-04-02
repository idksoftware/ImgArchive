#include "CSVMasterDatabase.h"
#include <memory>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "MetaType.h"
#include "SAUtils.h"
#include "CSVDBVisitor.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	CSVMasterDatabase::CSVMasterDatabase()
	{
	}


	CSVMasterDatabase::~CSVMasterDatabase()
	{
	}

	class MetadataPartition : public MTTable {
	public:
		MetadataPartition() : MTTable(new MetadataSchema) {};
		virtual ~MetadataPartition() {};
	};

	std::shared_ptr<CSVMasterDatabase> CSVMasterDatabase::m_this(0);
	std::string CSVMasterDatabase::m_dbpath;

	
	
	CSVMasterDatabase &CSVMasterDatabase::get() {
		static CSVMasterDatabase csvMasterDatabase;		
		return csvMasterDatabase;
	}

	void CSVMasterDatabase::setDBPath(const char *path) {
		m_dbpath = path;
	}



	void CSVMasterDatabase::add(MetadataObject &metadataObject, const char *relpath) {
		// check path exists
		if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
			if (SAUtils::mkDir(m_dbpath.c_str()) == false) {
				throw std::exception();
			}
		}

		std::string relPath = relpath;
		std::string dayStr = relPath.substr(0, 10);
		std::string yearStr = relPath.substr(0, 4);
		std::string imagename = relPath.substr(11, relPath.length() - 11);
		std::string fullPath = m_dbpath;
		fullPath += '/';
		fullPath += yearStr;
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				throw std::exception();
			}
		}
		fullPath += '/';
		fullPath += dayStr;
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				throw std::exception();
			}
		}


		MetadataPartition metadataPartition;
		std::string filename = metadataPartition.getSchema().getName() + ".csv";
		if (metadataPartition.read(fullPath.c_str(), filename.c_str()) == false) {
			if (ErrorCode::getErrorCode() != SIA_ERROR::OPEN_ERROR) {
				// file may not exist
				throw std::exception();
			}
		}
		if (metadataPartition.addRow(metadataObject) == false) {
			throw std::exception();
		}
		if (metadataPartition.write(fullPath.c_str(), filename.c_str()) == false) {
			throw std::exception();
		}

		//m_mirrorDB->process(relPath);
	}

	const MetadataObject *CSVMasterDatabase::get(unsigned int idx, const char *path) {
		return 0;
	}

	std::shared_ptr<MetadataObject> CSVMasterDatabase::get(const char *name, const char *path) {
		if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
			throw std::exception();
		}

		std::string relPath = path;
		std::string dayStr = relPath.substr(0, 10);
		std::string yearStr = relPath.substr(0, 4);
		std::string imagename = relPath.substr(11, relPath.length() - 11);
		std::string fullPath = m_dbpath;
		fullPath += '/';
		fullPath += yearStr;
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			throw std::exception();
		}
		fullPath += '/';
		fullPath += dayStr;
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			throw std::exception();
		}
		//MTDatabase &db = *m_mtDatabase;
		//MetadataSet metadataSet(db, fullPath.c_str());
		//return metadataSet.get(name);
		return std::make_shared<MetadataObject>();
	}

	bool CSVMasterDatabase::put(const char *name, const char *path, MetadataObject &mo) {
		std::string relPath = path;
		std::string dayStr = relPath.substr(0, 10);
		std::string yearStr = relPath.substr(0, 4);
		std::string imagename = relPath.substr(11, relPath.length() - 11);
		std::string fullPath = m_dbpath;
		fullPath += '/';
		fullPath += yearStr;
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			throw std::exception();
		}
		fullPath += '/';
		fullPath += dayStr;
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			throw std::exception();
		}

		//MTDatabase &db = *m_mtDatabase;
		//MetadataSet metadataSet(db, fullPath.c_str());
		//metadataSet.put(name, mo);
		return true;
	}

} // namespace simplearchive {
