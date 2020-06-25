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
#include "LogDocument.h"
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

	
	
	

	bool SystemHistoryResultsPresentation::writeHuman() {
		
		/*
	std::ofstream file;
	if (!m_filename.empty()) {
		file.open(m_filename.c_str());
		if (file.is_open() == false) {
			return false;
		}
		file << "\n---------------------------------------------------\n";
		file << "Image: " << m_title << '\n';
		file << "Path : " << m_description << '\n';
		file << "=====================================================\n";
		file << "Date Time             version     Event      Comment\n\n";
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			//std::cout << *i << '\n';
			CSVArgs csvArgs(',');
			if (csvArgs.process(i->c_str()) == false) {
				return false;
			}

			file << csvArgs.at(0) << "    ";
			file << csvArgs.at(1) << "      ";
			file << csvArgs.at(4) << "  ";
			file << csvArgs.at(3) << "\n\n";
		}
	}
	else {
	*/
	//std::cout << "\n---------------------------------------------------\n";
	//std::cout << "Image: " << m_title << '\n';
	//std::cout << "Path : " << m_description << '\n';
	//std::cout << "=====================================================\n";
	//std::cout << "Date Time             version     Event      Comment\n\n";
	
		size_t filename = 0;
		size_t filepath = 0;
		size_t comment = 0;

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			const MTTableSchema& ts = row->getSchema(); // testing only
			MTColumn& filenameCol = row->columnAt(DB_FILENAME);
			const std::string& filenameStr = filenameCol.getString();

			MTColumn& filepathCol = row->columnAt(DB_FILEPATH);
			const std::string& filepathStr = filepathCol.getString();

			MTColumn& commentCol = row->columnAt(DB_COMMENT);
			const std::string& commentStr = commentCol.getString();

			if (filenameStr.length() > filename) {
				filename = filenameStr.length();
			}

			if (filepathStr.length() > filepath) {
				filepath = filepathStr.length();
			}

			if (commentStr.length() > comment) {
				comment = commentStr.length();
			}
		}

		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
			MTSchema& columnInfo = *i;
			//printf("%s ", columnInfo.getName().c_str());
			std::cout << ' ';
			//	first = false;
			//}  setw
			if (columnInfo.getName().compare(DB_DATEADDED) == 0) {
				std::cout << std::setw(19) << columnInfo.getName().c_str();
			}
			else if (columnInfo.getName().compare(DB_FILENAME) == 0) {
				std::cout << std::setw(filename) << columnInfo.getName().c_str();
			}
			else if (columnInfo.getName().compare(DB_FILEPATH) == 0) {
				std::cout << std::setw(filepath) << columnInfo.getName().c_str();
			}
			else if (columnInfo.getName().compare(DB_COMMENT) == 0) {
				std::cout << std::setw(comment) << columnInfo.getName().c_str();
			}
			
			else {
				std::cout << columnInfo.getName().c_str();
			}
		}
		std::cout << '\n';
		//bool first = true;
		
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			auto schemaIdx = m_resultsList.getTableSchema().begin();
			for (auto i = row->begin(); i != row->end(); i++) {
				SharedMTColumn column = *i;
				MTSchema& columnInfo = *schemaIdx;
				//std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
				//std::cout << mtSchema->getName();
				//if (mtSchema == nullptr) {
				//	continue;
				//}
				//if (!first) {
				std::cout << ' ';
				//	first = false;
				//}  setw
				
				if (columnInfo.getName().compare(DB_FILENAME) == 0) {
					std::cout << std::setw(filename) << column->toString();
				} else if (columnInfo.getName().compare(DB_FILEPATH) == 0) {
					std::cout << std::setw(filepath) << column->toString();
				}
				else if (columnInfo.getName().compare(DB_COMMENT) == 0) {
					std::cout << std::setw(comment) << column->toString();
				}
				else if (columnInfo.getName().compare(DB_EVENT) == 0) {
					HistoryEvent::Event ev = (HistoryEvent::Event)column->getInt();
					HistoryEvent historyEvent(ev);
					std::cout << std::setw(10) << historyEvent.getString();
				}
				else {
					std::cout << column->toString();
				}
				schemaIdx++;
			}
			std::cout << '\n';
		}

		return true;
	}


	bool SystemHistoryResultsPresentation::writeJson() {
		return true;
	}

	bool SystemHistoryResultsPresentation::writeHtml()
	{
		return false;
	}

	bool SystemHistoryResultsPresentation::writeCSV() {
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			std::cout << *rowIt << '\n';
		}
		return true;
	}

	bool SystemHistoryResultsPresentation::writeXML() {
		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";


		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			std::cout << "\t<Event>\n";
			for (size_t i = 0; i != row->size(); i++) {
				MTTableSchema tableSchema = m_resultsList.getTableSchema();
				std::cout << writeTag(tableSchema.getColumnName(i).c_str(), row->columnAt(i).toString(), 2);
			}
			std::cout << "\t</Event>\n";
		}
		
		std::cout << "</Catalog>\n";
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
			const MTTableSchema& ts = row->getSchema(); // testing only
			printf("MTTableSchema: %s\n", ts.getName().c_str());
			//m_resultsList->push_back(row);
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
