#include "ImageHistory.h"
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
#include <fstream>
#include <iomanip>
#include <streambuf>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdarg>
#include "ImageHistory.h"
#include "LogName.h"
#include "ExifDateTime.h"
#include "SAUtils.h"
#include "History.h"
#include "CSVArgs.h"
#include "ArchivePath.h"
#include "ErrorCode.h"
#include "pathcontroller.h"
#include "ResultsList.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	// This class is used to write the log in normal operation. However reading the 
	// log will be done by the MTRow 

	ImageHistorySchema ImageHistoryRow::m_tableSchema;
	ImageHistoryRow::ImageHistoryRow() : MTRow(m_tableSchema) {}
	ImageHistoryRow::ImageHistoryRow(const char* filepath, const char* version, const char* comment, const HistoryEvent& he) : MTRow(m_tableSchema) {
		ExifDateTime date;
		date.now();
		columnAt(static_cast<int>(ImageHistoryIndex::IH_FILEPATH_IDX)).fromString(filepath);;
		columnAt(static_cast<int>(ImageHistoryIndex::IH_DATEADDED_IDX)).fromString(date.toString());;
		columnAt(static_cast<int>(ImageHistoryIndex::IH_VERSION_IDX)).fromString(version);;
		columnAt(static_cast<int>(ImageHistoryIndex::IH_EVENT_IDX)).fromString(he.getString());;
		columnAt(static_cast<int>(ImageHistoryIndex::IH_COMMENT_IDX)).fromString(comment);;
	}

	bool ImageHistoryPartition::findEvent(const char* event) {
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

	

	std::string ImageHistory::m_workspace;
	std::string ImageHistory::m_indexRoot;
	std::string ImageHistory::m_backup1;
	std::string ImageHistory::m_backup2;

	

	bool ImageHistory::init() {
		return true;
	}


	bool ImageHistory::newImage(const char* filename, const char* comment) {
		return add(filename, "0000", comment, HistoryEvent::Event::ADDED);
	}
	/**
	 * This function adds history to an image.
	 */
	bool ImageHistory::add(const char* filepath, int version, const char* comment, const HistoryEvent& he) {
		std::string buff = SAUtils::sprintf("%.4d", version);
		add(filepath, buff.c_str(), comment, he);
		return true;
	}
	/**
		filepath i.e 2015-11-10/DSC1236.jpg
	*/
	bool ImageHistory::add(const char* filepath, const char* version, const char* comment, const HistoryEvent& he) {

		ExifDateTime date;
		date.now();
		std::string dateStr = date.current();
		const char* event = he.getString();

		/*
		HistoryItem historyItem(dateStr.c_str(), version, filepath, comment, event);


		PathController workspaceController;
		workspaceController.setRoot(m_workspace.c_str());
		workspaceController.splitShort(filepath);
		workspaceController.makePath();
		std::string workspacePath = workspaceController.getFullPath();
		workspacePath += "/.imga/"; workspacePath += workspaceController.getImage();
		workspacePath += ".hst";

		if (writeLog(historyItem, workspacePath.c_str()) == false) {
			return false;
		}

		//PathController pathController;

		PathController indexController;
		indexController.setRoot(m_index.c_str());
		indexController.splitShort(filepath);
		indexController.makeImagePath();

		std::string indexPath = indexController.getFullPath();;
		
		indexPath += ".hst";

		if (writeLog(historyItem, indexPath.c_str()) == false) {
			return false;
		}
		*/

		ImageHistoryRow imageHistoryRow;
		PathController indexController;
		indexController.split(filepath);
		indexController.makeImagePath();
#ifdef _WIN32
		imageHistoryRow.columnAt(DB_FILENAME) = indexController.getImageName();
		imageHistoryRow.columnAt(DB_FILEPATH) = indexController.getYearday();
#else
		imageHistoryRow.columnAt(DB_FILENAME) = filepath.c_str();
		imageHistoryRow.columnAt(DB_FILEPATH) = filepath.c_str();
#endif
		imageHistoryRow.columnAt(DB_EVENT) = static_cast<int>(HistoryEvent::Event::ADDED);
		imageHistoryRow.columnAt(DB_VERSION) = 0;
		ExifDateTime dateAdded;
		dateAdded.now();
		imageHistoryRow.columnAt(DB_DATEADDED) = dateAdded;
		imageHistoryRow.columnAt(DB_COMMENT) = comment;



		// check path exists
		if (SAUtils::DirExists(m_indexRoot.c_str()) == false) {
			if (SAUtils::mkDir(m_indexRoot.c_str()) == false) {
				throw std::exception();
			}
		}
		std::string indexPath = m_indexRoot;
		PathController indexPathController(indexPath.c_str());
		indexPathController.splitShort(filepath);
		indexPathController.makeImagePath(false);
		indexPath += '/';
		indexPath += indexPathController.getYear();
		if (SAUtils::DirExists(indexPath.c_str()) == false) {
			SAUtils::mkDir(indexPath.c_str());
			if (SAUtils::DirExists(indexPath.c_str()) == false) {
				return false;
			}
		}
		indexPath += '/';
		indexPath += indexPathController.getYearday();
		if (SAUtils::DirExists(indexPath.c_str()) == false) {
			if (SAUtils::mkDir(indexPath.c_str()) == false) {
				return false;
			}
		}
		/*
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
		*/

		//ImageHistoryPartition imageHistoryPartition;

		std::string partitionFilename = indexController.getImageName() + ".hst";
		m_mtTablePartition->clear();
		/*
		if (imageHistoryPartition.read(indexPath.c_str(), partitionFilename.c_str()) == false) {
			if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
				// file may not exist
				return false;
			}
		}
		*/
		if (m_mtTablePartition->addRow(imageHistoryRow) == false) {
			return false;
		}
		if (m_mtTablePartition->write(indexPath.c_str(), partitionFilename.c_str()) == false) {
			return false;
		}
		return true;

	}

	bool ImageHistory::writeLog(HistoryItem& item, const char* path) {
		/*
		std::ofstream file;
		file.open(path, std::ios::out | std::ios::app);
		if (file.is_open() == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		file << item.toString().c_str() << '\n';
		file.close();
		*/
		return true;
	}

	/*
	ImageHistory& ImageHistory::get() {

		static ImageHistory imageHistory;
		return imageHistory;
	}
	*/

	bool ImageHistoryAction::onEnd()
	{
		/*
		if (!m_resultsList->write(ResultsList::FormatType::Human)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		*/
		return true;
	};

	bool ImageHistoryAction::onImage(const char* name)
	{

		std::string ext = SAUtils::getExtention(name);
		if (ext.compare("hst") == 0) {
			if (m_partition->read(name) == false) {
				return false;
			}

			for (auto i = m_partition->begin(); i != m_partition->end(); i++) {
				std::shared_ptr<MTRow> row = *i;
				m_resultsList->emplace_back(row);
			}
		}
		
		return true;
	}

	bool ImageHistoryAction::onStart()
	{
		m_resultsList = std::make_shared<ResultsList>(m_mtTableSchema);
		return true;
	};

	bool ImageHistoryAction::onDayFolder(const char* name)
	{
		return true;
	}



	



	bool ImageHistoryResultsPresentation::writeHuman() {

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
				}
				else if (columnInfo.getName().compare(DB_FILEPATH) == 0) {
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


	bool ImageHistoryResultsPresentation::writeJson() {
		return true;
	}

	bool ImageHistoryResultsPresentation::writeHtml()
	{
		return false;
	}

	bool ImageHistoryResultsPresentation::writeCSV() {
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			std::cout << *rowIt << '\n';
		}
		return true;
	}

	bool ImageHistoryResultsPresentation::writeXML() {
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


} /* namespace */