#include <string>
#include <vector>
#include <iomanip>
#include "SystemHistory.h"
//#include "ResultsList.h"
#include "MetaType.h"
#include "ResultsPresentation.h"
#include "pathcontroller.h"
#include "FileInfo.h"
#include "CIDKCrc.h"
#include "md5.h"
#include "SAUtils.h"
#include "LogName.h"

#include "HistoryEvent.h"
#include "ErrorCode.h"
//#include "LogDocument.h"
#include "AddressScope.h"
#include "IndexVisitor.h"
#include "CheckoutStatus.h"


namespace simplearchive {

	SystemHistorySchema SystemHistoryRow::m_tableSchema;

	SystemHistoryIndex::SystemHistoryIndex() : CSVIndexSystemHistory(std::make_shared<SystemHistoryAction>()) {}
	SystemHistoryIndex::~SystemHistoryIndex() {}

	bool SystemHistoryPartition::findEvent(const char *event) {
		if (MTTable::empty() == true) {
			return false;
		}
		const MTSchema info(MTSchema::Text, DB_FILENAME);
		MTColumn column(info);
		column.set(event);
		if (MTTable::find(column) == false) {
			return false;
		}
		return true;
	}
	
	void SystemHistory::setPath(const char* indexRoot) {
		m_indexRoot = indexRoot;
	}

	
	
	bool SystemHistory::add(const char *img, const char *comment) {
		std::string imagePath = img;
		PathController pathController(img);
		pathController.splitShort(img);
		SystemHistoryRow systemHistoryRow;

#ifdef WIN32
		systemHistoryRow.columnAt(DB_FILENAME) = pathController.getImage();
		systemHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday();
#else
		systemHistoryRow.columnAt(DB_FILENAME) = pathController.getImage().c_str();
		//tmp = pathController.getImage().c_str();
		systemHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday().c_str();
		//tmp = pathController.getYearday().c_str();
#endif
		systemHistoryRow.columnAt(DB_EVENT) = static_cast<int>(HistoryEvent::Event::ADDED);
		systemHistoryRow.columnAt(DB_VERSION) = 0;
		ExifDateTime dateAdded;
		dateAdded.now();
		systemHistoryRow.columnAt(DB_DATEADDED) = dateAdded;
		systemHistoryRow.columnAt(DB_COMMENT) = comment;
		std::string pidxPath = m_indexRoot;
		pidxPath += '/';
		pidxPath += pathController.getYear();
		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			if(SAUtils::mkDir(pidxPath.c_str()) == false) {
				return false;
			}
		}
		
		SystemHistoryPartition systemHistoryPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;
		
		if (SAUtils::FileExists(pidxPath.c_str()) == true) {
			systemHistoryPartition.read(pidxPath.c_str());
		}
		systemHistoryPartition.addRow(systemHistoryRow);
		systemHistoryPartition.write(pidxPath.c_str());

		return true;
	}

	SharedMTRow SystemHistory::getRow(const char *img) {
		std::string imagePath = img;

		PathController pathController(m_indexRoot.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return nullptr;
		}

		std::string pidxPath = m_indexRoot.c_str();
		pidxPath += '/'; pidxPath += pathController.getYear();
		if (SAUtils::DirExists(pidxPath.c_str()) == false) {
			return nullptr;
		}

		SystemHistoryPartition systemHistoryPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;

		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::FILE_NOT_FOUND);
			return nullptr;
		}
		if (systemHistoryPartition.read(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
			return nullptr;
		}
		if (systemHistoryPartition.findEvent(pathController.getImage().c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::IMAGE_NOT_FOUND);
			return nullptr;
		}
		SharedMTRow row = systemHistoryPartition.getCurrentRow();
		return row;
	}

	
	

	

	bool SystemHistory::add(const char* filepath, int version, const char* comment, const HistoryEvent& he)
	{
		ExifDateTime date;
		date.now();
		
		
		std::string imagePath = filepath;
		PathController pathController(filepath);
		pathController.splitShort(filepath);

		SystemHistoryRow systemHistoryRow;
#ifdef WIN32
		systemHistoryRow.columnAt(DB_FILENAME) = pathController.getImage();
		systemHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday();
#else
		systemHistoryRow.columnAt(DB_FILENAME) = pathController.getImage().c_str();
		//tmp = pathController.getImage().c_str();
		systemHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday().c_str();
		//tmp = pathController.getYearday().c_str();
#endif
		HistoryEvent::Event event = he.m_event;

		systemHistoryRow.columnAt(DB_EVENT) = static_cast<int>(event);
		systemHistoryRow.columnAt(DB_VERSION) = version;
		ExifDateTime dateAdded;
		dateAdded.now();
		systemHistoryRow.columnAt(DB_DATEADDED) = dateAdded;
		systemHistoryRow.columnAt(DB_COMMENT) = comment;

		return save(dateAdded, systemHistoryRow);

	}

	bool SystemHistory::newImage(const char* img, const char* comment) {
		std::string imagePath = img;
		PathController pathController(img);
		pathController.splitShort(img);
		SystemHistoryRow systemHistoryRow;

#ifdef WIN32
		systemHistoryRow.columnAt(DB_FILENAME) = pathController.getImage();
		systemHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday();
#else
		systemHistoryRow.columnAt(DB_FILENAME) = pathController.getImage().c_str();
		//tmp = pathController.getImage().c_str();
		systemHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday().c_str();
		//tmp = pathController.getYearday().c_str();
#endif
		systemHistoryRow.columnAt(DB_EVENT) = static_cast<int>(HistoryEvent::Event::ADDED);
		systemHistoryRow.columnAt(DB_VERSION) = 0;
		ExifDateTime dateAdded;
		dateAdded.now();
		systemHistoryRow.columnAt(DB_DATEADDED) = dateAdded;
		systemHistoryRow.columnAt(DB_COMMENT) = comment;

		return save(dateAdded, systemHistoryRow);
	}

	
	/*
		This uses the Image 

	*/
	bool SystemHistory::add(SystemHistoryRow & SystemHistoryRow, const char *img) {
		// check path exists
		if (SAUtils::DirExists(m_indexRoot.c_str()) == false) {
			if (SAUtils::mkDir(m_indexRoot.c_str()) == false) {
				throw std::exception();
			}
		}
		std::string indexPath = m_indexRoot;
		PathController indexPathController(indexPath.c_str());
		indexPathController.splitShort(img);
		indexPathController.makePath(false);
		indexPath += '/';
		indexPath += indexPathController.getYear();
		if (SAUtils::DirExists(indexPath.c_str()) == false) {
			SAUtils::mkDir(indexPath.c_str());
			if (SAUtils::DirExists(indexPath.c_str()) == false) {
				return false;
			}
		}
		std::string indexFullPath = indexPathController.getFullPath();
		if (SAUtils::DirExists(indexFullPath.c_str()) == false) {
			if (SAUtils::mkDir(indexFullPath.c_str()) == false) {
				throw std::exception();
			}
		}
		indexFullPath += '/';
		indexFullPath += indexPathController.getYearday();
		if (SAUtils::DirExists(indexFullPath.c_str()) == false) {
			if (SAUtils::mkDir(indexFullPath.c_str()) == false) {
				return false;
			}
		}


		SystemHistoryPartition systemHistoryPartition;
		std::string filename = SystemHistoryRow.getSchema().getName() + ".csv";
		if (systemHistoryPartition.read(indexFullPath.c_str(), filename.c_str()) == false) {
			if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
				// file may not exist
				return false;
			}
		}
		if (systemHistoryPartition.addRow(SystemHistoryRow) == false) {
			return false;
		}
		if (systemHistoryPartition.write(indexFullPath.c_str(), filename.c_str()) == false) {
			return false;
		}
		return true;
	}

	

	bool SystemHistoryAction::onEnd()
	{
		/*
		if (!m_resultsList->write(ResultsList::FormatType::Human)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		*/
		return true;
	};

	bool SystemHistoryAction::onImage(const char* name)
	{
		

		if (m_partition->read(name) == false) {
			return false;
		}

		for (auto i = m_partition->begin(); i != m_partition->end(); i++) {
			std::shared_ptr<MTRow> row = *i;
			m_resultsList->emplace_back(row);
		}
		/*
		for (auto i = m_resultsList->begin(); i != m_resultsList->end(); i++) {
			std::shared_ptr<MTRow> row = *i;
			const MTTableSchema& ts = row->getSchema(); // testing only
			printf("in list MTTableSchema: %s\n", ts.getName().c_str());
		}
		*/
		return true;
	}

	bool SystemHistoryAction::onStart()
	{
		m_resultsList = std::make_shared<ResultsList>(m_mtTableSchema);
		return true;
	};

	bool SystemHistoryAction::onDayFolder(const char* name)
	{
		return true;
	}
	
	
	

}
