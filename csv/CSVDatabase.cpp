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

#include <memory>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "CSVDatabase.h"
#include "MetaType.h"
#include "SAUtils.h"
#include "CSVDBVisitor.h"
#include "pathcontroller.h"
#include "DBDefines.h"
#include "MetadataObject.h"
#include "ResultsList.h"
#include "ResultsPresentation.h"
#include "CSVIndexAction.h"
#include "Clogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {


	class MetadataPartition : public MTTable {
	public:
		MetadataPartition() : MTTable(new MetadataSchema) {};
		virtual ~MetadataPartition() {};
	};
	
	class MasterMetadataAction : public CSVIndexAction {

	protected:

		std::shared_ptr<MetadataPartition> m_partition;
		std::shared_ptr<MetadataObject> m_currentRow;

		/// On the start of each directory found, this function is run.
		virtual bool onStart();
		/// At the end of each directory found, this function is run.
		virtual bool onEnd();
		/// On finding a file, this function is run.
		virtual bool onYearFolder(const char* name) { return true; };
		/// On finding a file, this function is run.
		virtual bool onYearEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onDayFolder(const char* name);
		/// On finding a directory, this function is run.
		virtual bool onDayEnd() { return true; };
		/// On finding a directory, this function is run.
		virtual bool onImage(const char* name);

		virtual bool onMetadata(const char* path, const char* name) { return true; };
		/// This function is a factory function used to create new FolderVisitor objects.

	public:
		/// Constructor
		MasterMetadataAction() : CSVIndexAction(std::make_shared<MetadataSchema>())
		{
			m_partition = std::make_shared<MetadataPartition>();
		};
		/// Distructor
		virtual ~MasterMetadataAction() = default;

	};


class MasterMatadataResultsPresentation : public ResultsPresentation {
public:
	MasterMatadataResultsPresentation(ResultsList& resultsList) : ResultsPresentation(resultsList) {};
	~MasterMatadataResultsPresentation() = default;

	bool writeHuman() override;
	bool writeXML() override;
	bool writeCSV() override;
	bool writeJson() override;
	bool writeHtml() override;
};

std::string CSVDatabase::m_dbpath;


CSVDatabase::CSVDatabase() : CSVTable(std::make_shared<MetadataSchema>(),
	std::make_shared<MetadataPartition>(),
	std::make_shared<CSVIndexCheckoutStatus>(std::make_shared<MasterMetadataAction>())
)
{};

CSVDatabase::~CSVDatabase() {
}

CSVDatabase &CSVDatabase::get() {
	static CSVDatabase csvDatabase;
	return csvDatabase;
}

void CSVDatabase::setDBPath(const char *path) {
	m_dbpath = path;
}



void CSVDatabase::add(MetadataObject &metadataObject, const char *relpath) {
	// check path exists
	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		if (SAUtils::mkDir(m_dbpath.c_str()) == false) {
			throw std::exception();
		}
	}

	PathController pathController(m_dbpath.c_str());
	pathController.splitShort(relpath);

	std::string fullPath = m_dbpath;
	fullPath += '/';
	fullPath += pathController.getYear();
	if (SAUtils::DirExists(fullPath.c_str()) == false) {
		if (SAUtils::mkDir(fullPath.c_str()) == false) {
			throw std::exception();
		}
	}

	MetadataPartition metadataPartition;
	std::string filename = pathController.getYearday() + ".csv";
	metadataPartition.read(fullPath.c_str(), filename.c_str());

	if (metadataPartition.addRow(metadataObject) == false) {
			throw std::exception();
	}
	if (metadataPartition.write(fullPath.c_str(), filename.c_str()) == false) {
		throw std::exception();
	}

	//m_mirrorDB->process(relPath);
}

const MetadataObject *CSVDatabase::get(unsigned int idx, const char *path) {
	return 0;
}

SharedMTRow CSVDatabase::get(const char *name, const char *path) {
	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		throw std::exception();
	}

	PathController pathController(m_dbpath.c_str());
	if (pathController.isValid() == false) {
		throw std::exception();
	}
	pathController.setRelativePath(path);
	
	pathController.makeImagePath();
	std::string fullPath = pathController.getFullPath();
	if (pathController.splitPathAndFile(fullPath.c_str()) == false) {
		throw std::exception();
	}
	MetadataPartition metadataPartition;
	std::string filename = metadataPartition.getSchema().getName() + ".csv";
	std::string rootPath = pathController.getRoot();
	std::string imagename = pathController.getImage();
	if (metadataPartition.read(rootPath.c_str(), filename.c_str()) == false) {
		if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
			// file may not exist
			throw std::exception();
		}
	}
	const MTSchema& info = metadataPartition.getSchema(static_cast<int>(MetadataObjectIndex::MD_FILENAME_IDX));
	MTColumn match(info);
	match.set(name);
	metadataPartition.find(match);
	SharedMTRow row = metadataPartition.getCurrentRow();
	SharedMTRow tmprow = metadataPartition.makeRow();
	tmprow->join(*row);
	return tmprow;
	
}

bool CSVDatabase::put(const char *name, const char *path, MetadataObject &mo) {
	std::string relPath = path;
	std::string dayStr = relPath.substr(0,10);
	std::string yearStr = relPath.substr(0,4);
	std::string imagename = relPath.substr(11,relPath.length() - 11);
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

bool CSVDatabase::showMasterMetadata(const char* addressScope, const char* aformatType, const char* filePath) {
	CLogger& logger = CLogger::getLogger();

	ResultsPresentation::FormatType formatType = ResultsPresentation::FormatType::unknown;

	if ((formatType = ResultsPresentation::parse(aformatType)) == ResultsPresentation::FormatType::unknown) {
		logger.log(LOG_OK, CLogger::Level::ERR, "Invalid format type: %s", aformatType);
		return false;
	}

	setPath(m_dbpath.c_str());
	if (select(addressScope) == false) {
		logger.log(LOG_OK, CLogger::Level::ERR, "Cannot process metadata");
		return false;
	}
	std::shared_ptr<ResultsList> results = getResults();
	if (results == nullptr) {
		logger.log(LOG_OK, CLogger::Level::WARNING, "No results for metadata");
		return false;
	}
	MasterMatadataResultsPresentation resultsPresentation(*results);
	if (!resultsPresentation.write(formatType)) {
		logger.log(LOG_OK, CLogger::Level::WARNING, "Cannot write metadata info");
		return false;
	}
	return true;
}

bool MasterMatadataResultsPresentation::writeHuman() {

	MasterDatabaseWriteHuman masterDatabaseWriteHuman(m_resultsList);
	if (!masterDatabaseWriteHuman.write()) {
		return false;
	}
	return true;

}

bool MasterMatadataResultsPresentation::writeXML() {
	std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<Catalog ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

	for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
		SharedMTRow row = *rowIt;
		std::cout << "\t<Image>\n";
		for (size_t i = 0; i != row->size(); i++) {
			MTTableSchema tableSchema = m_resultsList.getTableSchema();
			std::cout << writeTag(tableSchema.getColumnName(i).c_str(), row->columnAt((int)i).toString(), 2);
		}
		std::cout << "\t</Image>\n";
	}
	std::cout << "</Catalog>\n";
	return true;
}

bool MasterMatadataResultsPresentation::writeCSV() {
	return true;
}

bool MasterMatadataResultsPresentation::writeJson() {
	return true;
}

bool MasterMatadataResultsPresentation::writeHtml() {
	return true;
}


bool MasterMetadataAction::onEnd()
{
	/*
	if (!m_resultsList->write(ResultsList::FormatType::Human)) {
		ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
		return false;
	}
	*/
	return true;
}

bool MasterMetadataAction::onImage(const char* name)
{
	/*
	if (m_partition->read(name) == false) {
		return false;
	}

	for (auto i = m_partition->begin(); i != m_partition->end(); i++) {
		std::shared_ptr<MTRow> row = *i;
		m_resultsList->emplace_back(row);
	}
	*/
	return true;
}

bool MasterMetadataAction::onStart()
{
	m_resultsList = std::make_shared<ResultsList>(m_mtTableSchema);
	return true;
};

bool MasterMetadataAction::onDayFolder(const char* name)
{
	m_partition = std::make_shared<MetadataPartition>();

	if (m_partition->read(name) == false) {
		return false;
	}

	for (auto i = m_partition->begin(); i != m_partition->end(); i++) {
		std::shared_ptr<MTRow> row = *i;
		std::string path = row->columnAt(DB_FILEPATH).getString();
		path += '/';
		std::string image = row->columnAt(DB_FILENAME).getString();
		path += image;
		if (!m_addressScope->isImageInScope(image.c_str())) {
			continue;
		}
		onImage(path.c_str());
		m_resultsList->emplace_back(row);



	}
	return true;
}





class CSVDBCopyAction : public CSVDBAction {
	std::string m_copyPath;
	std::string m_sourcePath;
	std::string m_currCopyPath;
	std::string m_currSourcePath;
	std::string m_currYear;
protected:
        /// On the start of each directory found, this function is run.
	virtual void onStart() {
		m_currCopyPath = m_copyPath;
		m_currSourcePath = m_sourcePath;
		//printf("onStart %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
	};
	   /// At the end of each directory found, this function is run.
	virtual void onEnd() {
		DEBUG_PRINT("onEnd\n");
	};
        /// On finding a file, this function is run.
	virtual void onYearFolder(const char *name) {
		m_currYear = name;
		m_currCopyPath += '/';
		m_currCopyPath += name;
		m_currSourcePath += '/';
		m_currSourcePath += name;
		//printf("onYearFolder %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		if (SAUtils::DirExists(m_currCopyPath.c_str()) == false) {
			if (SAUtils::mkDir(m_currCopyPath.c_str()) == false) {
				throw std::exception();
			}
		}
		//printf("onYearFolder %s: \n", name);
	};
    /// On finding a file, this function is run.
	virtual void onYearEnd() {
		DEBUG_PRINT("onYearEnd\n");
	};
        /// On finding a directory, this function is run.
	virtual void onDayFolder(const char *name) {
		m_currCopyPath += '/';
		m_currCopyPath += name;
		m_currSourcePath += '/';
		m_currSourcePath += name;
		//printf("onDayFolder %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		if (SAUtils::DirExists(m_currCopyPath.c_str()) == false) {
			if (SAUtils::mkDir(m_currCopyPath.c_str()) == false) {
				throw std::exception();
			}
		}
		//printf("onDayFolder %s: \n", name);
	};
    /// On finding a directory, this function is run.
	virtual void onDayEnd() {
		m_currCopyPath = m_copyPath;
		m_currSourcePath = m_sourcePath;
		m_currCopyPath += '/';
		m_currCopyPath += m_currYear;
		m_currSourcePath += '/';
		m_currSourcePath += m_currYear;
		//printf("onDayEnd %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		//printf("onDayEnd\n");
	};
	   /// On finding a directory, this function is run.
	virtual void onDBFile(const char *path, const char *name) {
		std::string currCopyPath = m_currCopyPath;
		currCopyPath += '/';
		currCopyPath += name;
		std::string currSourcePath = m_currSourcePath;
		currSourcePath += '/';
		currSourcePath += name;
		//printf("tonDBFile %s %s\n", currSourcePath.c_str(), currCopyPath.c_str());
		if (SAUtils::copy(currSourcePath.c_str(), currCopyPath.c_str()) == false) {
			throw std::exception();
		}
		//printf("\t\tonDBFile %s: %s\n", path, name);

	};
public:
	CSVDBCopyAction(const char *sourcePath, const char *copyPath) {
		m_copyPath = copyPath;
		m_sourcePath = sourcePath;
	}
};

class CSVDBValidateAction : public CSVDBAction {
	std::string m_copyPath;
	std::string m_sourcePath;
	std::string m_currCopyPath;
	std::string m_currSourcePath;
	std::string m_currYear;
protected:
        /// On the start of each directory found, this function is run.
	virtual void onStart() {
		m_currCopyPath = m_copyPath;
		m_currSourcePath = m_sourcePath;
		//printf("onStart %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
	};
	   /// At the end of each directory found, this function is run.
	virtual void onEnd() {
		printf("onEnd\n");
	};
        /// On finding a file, this function is run.
	virtual void onYearFolder(const char *name) {
		m_currYear = name;
		m_currCopyPath += '/';
		m_currCopyPath += name;
		m_currSourcePath += '/';
		m_currSourcePath += name;
		//printf("onYearFolder %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		if (SAUtils::DirExists(m_currCopyPath.c_str()) == false) {
			printf("Validate failed %s\n", m_currCopyPath.c_str());
		}
		//printf("onYearFolder %s: \n", name);
	};
    /// On finding a file, this function is run.
	virtual void onYearEnd() {
		printf("onYearEnd\n");
	};
        /// On finding a directory, this function is run.
	virtual void onDayFolder(const char *name) {
		m_currCopyPath += '/';
		m_currCopyPath += name;
		m_currSourcePath += '/';
		m_currSourcePath += name;
		//printf("onDayFolder %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		if (SAUtils::DirExists(m_currCopyPath.c_str()) == false) {
			printf("Validate failed %s\n", m_currCopyPath.c_str());
		}
		//printf("onDayFolder %s: \n", name);
	};
    /// On finding a directory, this function is run.
	virtual void onDayEnd() {
		m_currCopyPath = m_copyPath;
		m_currSourcePath = m_sourcePath;
		m_currCopyPath += '/';
		m_currCopyPath += m_currYear;
		m_currSourcePath += '/';
		m_currSourcePath += m_currYear;
		//printf("onDayEnd %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		//printf("onDayEnd\n");
	};
	   /// On finding a directory, this function is run.
	virtual void onDBFile(const char *path, const char *name) {
		std::string currCopyPath = m_currCopyPath;
		currCopyPath += '/';
		currCopyPath += name;
		std::string currSourcePath = m_currSourcePath;
		currSourcePath += '/';
		currSourcePath += name;
		printf("tonDBFile %s %s\n", currSourcePath.c_str(), currCopyPath.c_str());
		if (SAUtils::verify(currSourcePath.c_str(), currCopyPath.c_str()) == false) {
			printf("Validate failed %s\n", m_currCopyPath.c_str());
		}
		//printf("\t\tonDBFile %s: %s\n", path, name);

	};
public:
	CSVDBValidateAction(const char *sourcePath, const char *copyPath) {
		m_copyPath = copyPath;
		m_sourcePath = sourcePath;
	}
};

bool CSVDatabase::copy(const char *path) {
	CSVDBVisitor csvdbVisitor(new CSVDBCopyAction(m_dbpath.c_str(), path));
	csvdbVisitor.process(m_dbpath.c_str());
	return true;
}

bool CSVDatabase::validate(const char *path) {
	CSVDBVisitor csvdbVisitor(new CSVDBValidateAction(m_dbpath.c_str(), path));
	csvdbVisitor.process(m_dbpath.c_str());
	return true;
}
/*
std::shared_ptr<MetadataObject> CSVDatabase::get(const char *name, const char *path) {
	MetadataSet metadataSet(path);
	return MetadataSet.get(name);
}
*/

/*
class MirrorDB {
	std::string m_path;
	std::string m_primary;
	std::string m_backup1;
	std::string m_backup2;
	std::string m_master;
	std::string makeFolders(std::string &pasePath, std::string &relPath);
	bool copy(std::string &from, std::string &to);
	bool copyDB(std::string &from, std::string &to, const char *name);

public:
	MirrorDB(const char *rootPath);
	~MirrorDB() {};
	bool process(std::string &shortPath);
};
*/

/*
class MetadataSet {

	MTTable& m_Metadata;


	std::string m_path;
	MTDatabase &m_db;
public:
	MetadataSet(MTDatabase db, const char *path) : m_db(db), m_path(path),
		m_Metadata(db.getTable(MetadataTable)) {}
	~MetadataSet() {};
	void add(MetadataObject &metadataObject);
	std::shared_ptr<MetadataObject> get(const char *image);
	bool put(const char *image, MetadataObject &mo);
};


void MetadataSet::add(MetadataObject &metadataObject) {

	std::string filename = m_Metadata.getSchema().getName() + ".csv";
	m_Metadata.read(m_path.c_str(), filename.c_str());
	m_Metadata.addRow(metadataObject);
	m_Metadata.write(m_path.c_str(), filename.c_str());



}

std::shared_ptr<MetadataObject> MetadataSet::get(const char *image) {
	std::shared_ptr<MetadataObject> mo(new MetadataObject);

	m_assetProperties.read(m_path.c_str());


	unsigned int row = m_assetProperties.findImage(image, 1);
	if (row == std::string::npos) {
		mo.reset();
		return mo;
	}

	m_assetProperties.load(row, *mo);
	m_cameraInformation.read(m_path.c_str());
	m_cameraInformation.load(row, *mo);
	m_GPSProperties.read(m_path.c_str());
	m_GPSProperties.load(row, *mo);
	m_copyrightProperties.read(m_path.c_str());
	m_copyrightProperties.load(row, *mo);
	m_MediaProperties.read(m_path.c_str());
	m_MediaProperties.load(row, *mo);

	return mo;
}

bool MetadataSet::put(const char *image, MetadataObject &mo) {

	m_assetProperties.read(m_path.c_str());
	unsigned int row = m_assetProperties.findImage(image, 1);
	if (row == std::string::npos) {
		return false;
	}
	m_assetProperties.save(row, mo);
	m_cameraInformation.save(row, mo);
	m_GPSProperties.save(row, mo);
	m_copyrightProperties.save(row, mo);
	m_MediaProperties.save(row, mo);

	return true;
}

*/

/*
MirrorDB::MirrorDB(const char *rootPath) {
	m_path = rootPath;

	if (ArchivePath::isMasterEnabled() == true) {
		m_master = ArchivePath::getMaster().getCSVDatabasePath();
	}
	if (ArchivePath::isMasterBackup1Enabled() == true) {
		m_backup1 = ArchivePath::getMasterBackup1().getCSVDatabasePath();

	}
	if (ArchivePath::isMasterBackup2Enabled() == true) {
		m_backup2 = ArchivePath::getMasterBackup2().getCSVDatabasePath();;
	}

}

bool MirrorDB::copyDB(std::string &from, std::string &to, const char *name) {
	std::string fromfull = from;
	fromfull += '/'; fromfull += name;
	std::string tofull = to;
	tofull += '/'; tofull += name;
	if (SAUtils::copy(fromfull.c_str(), tofull.c_str()) == false) {
		return false;
	}
	return true;
}

bool MirrorDB::copy(std::string &from, std::string &to) {
	if (copyDB(from, to, MetadataTableFilename) == false) {
		return false;
	}

	return true;
}

std::string MirrorDB::makeFolders(std::string &basePath, std::string &relPath) {
	std::string dayStr = relPath.substr(0, 10);
	std::string yearStr = relPath.substr(0, 4);

	std::string fullPath = basePath;

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
	return fullPath;
}

bool MirrorDB::process(std::string &relPath) {

	std::string dayStr = relPath.substr(0, 10);
	std::string yearStr = relPath.substr(0, 4);
	std::string imagename = relPath.substr(11, relPath.length() - 11);
	std::string fromfull = m_path;
	fromfull += '/'; fromfull += yearStr;
	fromfull += '/'; fromfull += dayStr;


	if (ArchivePath::isMasterEnabled() == true) {
		std::string fullPath = makeFolders(m_master, relPath);
		if (copy(fromfull, fullPath) == false) {
			return false;
		}
	}
	if (ArchivePath::isMasterBackup1Enabled() == true) {
		std::string fullPath = makeFolders(m_backup1, relPath);
		if (copy(fromfull, fullPath) == false) {
			return false;
		}
	}
	if (ArchivePath::isMasterBackup2Enabled() == true) {
		std::string fullPath = makeFolders(m_backup2, relPath);
		if (copy(fromfull, fullPath) == false) {
			return false;
		}
	}

	return true;
}
*/
/********************************/



} /* namespace simplearchive */
