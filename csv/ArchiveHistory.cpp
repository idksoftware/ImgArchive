#include <string>
#include "ArchiveHistory.h"
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

	ArchiveHistorySchema ArchiveHistoryRow::m_tableSchema;

	class ArchiveHistoryResultsPresentation : public ResultsPresentation {
	public:
		ArchiveHistoryResultsPresentation(ResultsList& resultsList) : ResultsPresentation(resultsList) {};
		~ArchiveHistoryResultsPresentation() = default;

		bool writeHuman() override;
		bool writeXML() override;
		bool writeCSV() override;
		bool writeJson() override;
		bool writeHtml() override;
	};

	bool ArchiveHistoryPartition::findEvent(const char *event) {
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
	
	
	

	void ArchiveHistory::setPath(const char* indexRoot) {
		ArchiveHistory &archiveHistory = ArchiveHistory::get();
		archiveHistory.m_indexRoot = indexRoot;
	}

	
	
	bool ArchiveHistory::add(const char *img, const char *comment) {
		std::string imagePath = img;
		PathController pathController(img);
		pathController.splitShort(img);
		ArchiveHistoryRow archiveHistoryRow;

#ifdef WIN32
		archiveHistoryRow.columnAt(DB_FILENAME) = pathController.getImage();
		archiveHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday();
#else
		archiveHistoryRow.columnAt(DB_FILENAME) = pathController.getImage().c_str();
		//tmp = pathController.getImage().c_str();
		archiveHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday().c_str();
		//tmp = pathController.getYearday().c_str();
#endif
		archiveHistoryRow.columnAt(DB_EVENT) = static_cast<int>(HistoryEvent::Event::ADDED);
		archiveHistoryRow.columnAt(DB_VERSION) = 0;
		ExifDateTime dateAdded;
		dateAdded.now();
		archiveHistoryRow.columnAt(DB_DATEADDED) = dateAdded;

		std::string pidxPath = m_indexRoot;
		pidxPath += '/';
		pidxPath += pathController.getYear();
		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			if(SAUtils::mkDir(pidxPath.c_str()) == false) {
				return false;
			}
		}
		
		ArchiveHistoryPartition archiveHistoryPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;
		
		if (SAUtils::FileExists(pidxPath.c_str()) == true) {
			archiveHistoryPartition.read(pidxPath.c_str());
		}
		archiveHistoryPartition.addRow(archiveHistoryRow);
		archiveHistoryPartition.write(pidxPath.c_str());

		return true;
	}

	SharedMTRow ArchiveHistory::getRow(const char *img) {
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


		

		ArchiveHistoryPartition archiveHistoryPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;

		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::FILE_NOT_FOUND);
			return nullptr;
		}
		if (archiveHistoryPartition.read(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
			return nullptr;
		}
		if (archiveHistoryPartition.findEvent(pathController.getImage().c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::IMAGE_NOT_FOUND);
			return nullptr;
		}
		SharedMTRow row = archiveHistoryPartition.getCurrentRow();
		return row;
	}

	ArchiveHistory& ArchiveHistory::get() {
		
		static ArchiveHistory archiveHistory;
		return archiveHistory;
	}
	

	

	bool ArchiveHistory::add(const char* filepath, int version, const char* comment, const HistoryEvent& he)
	{
		ExifDateTime date;
		date.now();
		
		
		std::string imagePath = filepath;
		PathController pathController(filepath);
		pathController.splitShort(filepath);

		ArchiveHistoryRow archiveHistoryRow;
#ifdef WIN32
		archiveHistoryRow.columnAt(DB_FILENAME) = pathController.getImage();
		archiveHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday();
#else
		archiveHistoryRow.columnAt(DB_FILENAME) = pathController.getImage().c_str();
		//tmp = pathController.getImage().c_str();
		archiveHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday().c_str();
		//tmp = pathController.getYearday().c_str();
#endif
		HistoryEvent::Event event = he.m_event;

		archiveHistoryRow.columnAt(DB_EVENT) = static_cast<int>(event);
		archiveHistoryRow.columnAt(DB_VERSION) = version;
		ExifDateTime dateAdded;
		dateAdded.now();
		archiveHistoryRow.columnAt(DB_DATEADDED) = dateAdded;

		return save(dateAdded, archiveHistoryRow);

	}

	bool ArchiveHistory::newImage(const char* img, const char* comment) {
		std::string imagePath = img;
		PathController pathController(img);
		pathController.splitShort(img);
		ArchiveHistoryRow archiveHistoryRow;

#ifdef WIN32
		archiveHistoryRow.columnAt(DB_FILENAME) = pathController.getImage();
		archiveHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday();
#else
		archiveHistoryRow.columnAt(DB_FILENAME) = pathController.getImage().c_str();
		//tmp = pathController.getImage().c_str();
		archiveHistoryRow.columnAt(DB_FILEPATH) = pathController.getYearday().c_str();
		//tmp = pathController.getYearday().c_str();
#endif
		archiveHistoryRow.columnAt(DB_EVENT) = static_cast<int>(HistoryEvent::Event::ADDED);
		archiveHistoryRow.columnAt(DB_VERSION) = 0;
		ExifDateTime dateAdded;
		dateAdded.now();
		archiveHistoryRow.columnAt(DB_DATEADDED) = dateAdded;
		return save(dateAdded, archiveHistoryRow);
	}

	

	bool ArchiveHistory::add(ArchiveHistoryRow & archiveHistoryRow, const char *img) {
		// check path exists
		if (SAUtils::DirExists(m_indexRoot.c_str()) == false) {
			if (SAUtils::mkDir(m_indexRoot.c_str()) == false) {
				throw std::exception();
			}
		}
		std::string checkoutStatusFileRoot = m_indexRoot;
		PathController chkoutPathController(checkoutStatusFileRoot.c_str());
		chkoutPathController.splitShort(img);
		chkoutPathController.makePath(false);
		checkoutStatusFileRoot += '/';
		checkoutStatusFileRoot += chkoutPathController.getYear();
		if (SAUtils::DirExists(checkoutStatusFileRoot.c_str()) == false) {
			SAUtils::mkDir(checkoutStatusFileRoot.c_str());
			if (SAUtils::DirExists(checkoutStatusFileRoot.c_str()) == false) {
				return false;
			}
		}
		std::string chkoutPath = chkoutPathController.getFullPath();
		if (SAUtils::DirExists(chkoutPath.c_str()) == false) {
			if (SAUtils::mkDir(chkoutPath.c_str()) == false) {
				throw std::exception();
			}
		}
		chkoutPath += '/';
		chkoutPath += chkoutPathController.getYearday();
		if (SAUtils::DirExists(chkoutPath.c_str()) == false) {
			if (SAUtils::mkDir(chkoutPath.c_str()) == false) {
				return false;
			}
		}


		ArchiveHistoryPartition archiveHistoryPartition;
		std::string filename = archiveHistoryRow.getSchema().getName() + ".csv";
		if (archiveHistoryPartition.read(chkoutPath.c_str(), filename.c_str()) == false) {
			if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
				// file may not exist
				return false;
			}
		}
		if (archiveHistoryPartition.addRow(archiveHistoryRow) == false) {
			return false;
		}
		if (archiveHistoryPartition.write(chkoutPath.c_str(), filename.c_str()) == false) {
			return false;
		}
		return true;
		//m_mirrorDB->process(relPath);
	}

	bool ArchiveHistory::save(ExifDateTime dateAdded, ArchiveHistoryRow archiveHistoryRow)
	{
		std::string dateStr = dateAdded.toLogString();

		std::string yearStr = dateStr.substr(0, 4);
		std::string monthStr = dateStr.substr(5, 2);
		std::string dayStr = dateStr.substr(8, 2);

		std::string addressStr = yearStr + '-' + monthStr + '-' + dayStr;

		if (SAUtils::DirExists(m_indexRoot.c_str()) == false) {
			return false;
		}

		std::string indexPath = m_indexRoot + '/' + yearStr;

		if (SAUtils::DirExists(indexPath.c_str()) == false) {
			if (SAUtils::mkDir(indexPath.c_str()) == false) {
				return false;
			}
		}

		indexPath += '/' + addressStr;

		if (SAUtils::DirExists(indexPath.c_str()) == false) {
			if (SAUtils::mkDir(indexPath.c_str()) == false) {
				return false;
			}
		}

		LogName logName;
		std::string currentFilename = logName.makeName(indexPath.c_str(), "", "hst", 1);

		indexPath += '/' + currentFilename;

		ArchiveHistoryPartition archiveHistoryPartition;
		if (SAUtils::FileExists(indexPath.c_str()) == true) {
			archiveHistoryPartition.read(indexPath.c_str());
		}
		archiveHistoryPartition.addRow(archiveHistoryRow);
		archiveHistoryPartition.write(indexPath.c_str());

		return true;
	}

	
	

	bool ArchiveHistoryResultsPresentation::writeHuman() {
		
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

		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
			MTSchema& columnInfo = *i;
			printf("%s ", columnInfo.getName().c_str());

		}
		printf("\n");
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			for (auto i = row->begin(); i != row->end(); i++) {
				SharedMTColumn column = *i;
				//std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
				//std::cout << mtSchema->getName();
				//if (mtSchema == nullptr) {
				//	continue;
				//}
				std::cout << ',';
				std::cout << column->toString();

			}
			std::cout << '\n';
		}
		//}
		return true;
	}


	bool ArchiveHistoryResultsPresentation::writeJson() {
		return true;
	}

	bool ArchiveHistoryResultsPresentation::writeHtml()
	{
		return false;
	}

	bool ArchiveHistoryResultsPresentation::writeCSV() {
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			std::cout << *rowIt << '\n';
		}
		return true;
	}

	bool ArchiveHistoryResultsPresentation::writeXML() {
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
	


	/*
	bool ArchiveHistoryIndex::setScope(const char* scope) {

		std::string scopeStr;
		if (scope != nullptr) {
			std::string scopeStr = scope;
		}
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

	bool ArchiveHistoryIndex::process() {
		if (!process(m_indexPath.c_str())) {
			return false;
		}
		return true;
	}

	*/

	/*
	ArchiveHistoryIndex::ArchiveHistoryIndex(std::shared_ptr<ArchiveHistoryAction> indexAction) {
		m_indexAction = indexAction;
		m_addressScope = std::make_unique<AddressScope>();
	}

	ArchiveHistoryIndex::~ArchiveHistoryIndex() {

	}


	bool ArchiveHistoryIndex::process(const char* rootFolder) {
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
			catch (std::exception ) { //e*) {
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

	*/

/*
	bool ArchiveHistoryAction::onEnd()
	{
		if (!resultsList->write(ResultsList::FormatType::Human)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	};

	bool ArchiveHistoryAction::onImage(const char* name)
	{
		std::shared_ptr<ArchiveHistoryPartition> currentPartition = std::make_shared<ArchiveHistoryPartition>();

		if (currentPartition->read(name) == false) {
			return false;
		}

		for (auto i = currentPartition->begin(); i != currentPartition->end(); i++) {
			std::shared_ptr<MTRow> row = *i;
			resultsList->push_back(row);

		}
		return true;
	}

	bool ArchiveHistoryAction::onStart()
	{
		resultsList = std::make_shared<ResultsList>(m_mtTableSchema);
		return true;
	};

	bool ArchiveHistoryAction::onDayFolder(const char* name)
	{
		return true;
	}
	
	*/
}
