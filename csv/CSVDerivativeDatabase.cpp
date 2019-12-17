#include "CSVDerivativeDatabase.h"
#include "DerivativeMetadata.h"
#include <memory>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "MetaType.h"
#include "SAUtils.h"
#include "CSVDBVisitor.h"
#include "pathcontroller.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	/*
	DerivativeMetadataSchema DerivativeMetadata::m_schema;
	DerivativeMetadata::DerivativeMetadata() : MTRow(m_schema)
	{
	}


	DerivativeMetadata::~DerivativeMetadata()
	{
	}



	

	class DerivativePartition : public MTTable {
	public:
		DerivativePartition() : MTTable(new DerivativeMetadataSchema) {};
		virtual ~DerivativePartition() {};
	};
	*/

	CSVDerivativeDatabase::CSVDerivativeDatabase()
	{
	}


	CSVDerivativeDatabase::~CSVDerivativeDatabase()
	{
	}

	std::shared_ptr<CSVDerivativeDatabase> CSVDerivativeDatabase::m_this(0);
	std::string CSVDerivativeDatabase::m_dbpath;



	CSVDerivativeDatabase &CSVDerivativeDatabase::get() {
		static CSVDerivativeDatabase cvsDerivativeDatabase;
		return cvsDerivativeDatabase;
	}

	void CSVDerivativeDatabase::setDBPath(const char *path) {
		m_dbpath = path;
	}



	void CSVDerivativeDatabase::add(DerivativeMetadata &metadataObject) {
		// check path exists

		std::string relpath = metadataObject.columnAt(DB_VERSIONPATH).getString();

		if (PathController::validateYYMMDD(relpath.c_str()) == false) {
			throw std::exception();
		}
		if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
			if (SAUtils::mkDir(m_dbpath.c_str()) == false) {
				throw std::exception();
			}
		}

		std::string relPath = relpath;
		std::string dayStr = relPath.substr(0, 10);
		std::string yearStr = relPath.substr(0, 4);
		std::string imagename = metadataObject.columnAt(DB_FILENAME).getString();
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

		DerivativePartition partition;
		std::string filename = partition.getSchema().getName() + ".csv";
		if (partition.read(fullPath.c_str(), filename.c_str()) == false) {
			if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
				// file may not exist
				throw std::exception();
			}
		}
		if (partition.addRow(metadataObject) == false) {
			throw std::exception();
		}
		if (partition.write(fullPath.c_str(), filename.c_str()) == false) {
			throw std::exception();
		}

		//m_mirrorDB->process(relPath);
	}

	const DerivativeMetadata *CSVDerivativeDatabase::get(unsigned int idx, const char *path) {
		return 0;
	}

	std::shared_ptr<DerivativeMetadata> CSVDerivativeDatabase::get(const char *name, const char *path) {
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
		return std::make_shared<DerivativeMetadata>();
	}

	std::shared_ptr<DerivativeMetadata> CSVDerivativeDatabase::makeRow() {
		return std::make_shared<DerivativeMetadata>();
	}

	bool CSVDerivativeDatabase::put(const char *name, const char *path, DerivativeMetadata &mo) {
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
