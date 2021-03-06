#include <string>
#include "CheckoutStatus.h"
#include "CheckDisk.h"
//#include "FolderList.h"
#include "pathcontroller.h"
#include "FileInfo.h"
#include "CIDKCrc.h"
#include "md5.h"

#include "HistoryEvent.h"
#include "ErrorCode.h"
#include "ResultsPresentation.h"
#include "CheckoutStatusResultsPresentation.h"
#include "AddressScope.h"
#include "IndexVisitor.h"
#include "ResultsList.h"
#include "CLogger.h"
#include <iomanip>


namespace simplearchive {

	
	CheckoutStatus::CheckoutStatus() : CSVTable(std::make_shared<CheckoutSchema>(),
		std::make_shared<CheckoutStatusPartition>(),
		std::make_shared<CSVIndexCheckoutStatus>(std::make_shared<CheckoutStatusAction>())
	)
	{};
	
	CheckoutStatus::CheckoutStatus(std::shared_ptr<CheckoutStatusAction> indexAction) : CSVTable(std::make_shared<CheckoutSchema>(),
		std::make_shared<CheckoutStatusPartition>(),
		std::make_shared<CSVIndexCheckoutStatus>(indexAction)
	)
	{};

	/*
	class ScopeIterator {
		std::string m_rootPath;
		std::vector<std::string>::iterator m_yearIterator;
		std::vector<std::string>::iterator m_daylistIterator;
		std::shared_ptr<CheckoutPartition> m_currentPartition;
		CheckoutPartition::iterator m_currentPos;
		std::string m_currentAddress;
		AddressScope m_addressScope;
		FileList_Ptr m_yearlist;
		FileList_Ptr m_daylist;
	public:
		ScopeIterator(const std::string rootPath) {
			m_rootPath = rootPath;
		}
		
		///	Initalises the addressing scope.
		bool initScope(const char *scope);
		
	    /// Processes the scope. returns false when no more to process.
		///	throws an exeption on error.
		bool processScope(const std::string rootPath);
		
		///	returns the next address processed by processScope.
		std::string& nextAddress();

		
	};

	bool ScopeIterator::initScope(const char *scope) {
		std::string scopeStr = scope;

		if (scopeStr.empty()) {
			m_addressScope.scopeAll();
		}
		else {
			if (!m_addressScope.scope(scope)) {
				return false;
			}
		}
		m_yearlist = SAUtils::getFiles_(m_rootPath.c_str());
		m_yearIterator = m_yearlist->begin();

		return true;
	}

	bool ScopeIterator::processScope(const std::string rootPath) {
		std::string dataPath = m_rootPath;
	
	tryAgain:
		if (m_yearIterator != m_yearlist->end()) { 
			std::string dataString = *m_yearIterator;
			if (dataString[0] == '.') {
				goto tryAgain;
			}

			std::string year = dataString.substr(0, 4);

			if (year.compare("system") == 0) {
				goto tryAgain;
			}
			//printf("Year found %s\n", year.c_str());
			std::string yearPath = dataPath += '/';
			yearPath += year;

			if (!m_addressScope.isInScope(year.c_str())) {
				goto tryAgain;
			}

			m_daylist = SAUtils::getFiles_(yearPath.c_str());
			for (std::vector<std::string>::iterator m_daylistIterator = m_daylist->begin(); m_daylistIterator != m_daylist->end(); m_daylistIterator++) {

				std::string dataString = *i;
				if (dataString[0] == '.') {
					continue;
				}
				std::string dateStr = dataString.substr(0, 10);
				if (!m_addressScope.isInScope(dateStr.c_str())) {
					continue;
				}
				std::string filepath = yearPath + "/" + dataString;

				printf("%s\n", filepath.c_str());
				//processFile(log, filepath);
				m_currentPartition = std::make_shared<CheckoutPartition>();
				if (m_currentPartition->read(filepath.c_str()) == false) {
					if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
						throw std::exception("Cannot read current partition");
					}
				}
				CheckoutPartition::iterator m_currentPos = m_currentPartition->begin();


				for (m_currentPos; m_currentPos != m_currentPartition->end(); m_currentPos++) {
					std::string s;
					s = (*i)->toString();
					log->push_back(s);
					//printf("%s\n", (*i)->toString().c_str());
				}

				return true;
			}
			m_yearIterator++;
		}
	}
	*/

	
	CheckoutSchema CheckoutRow::m_tableSchema;

	bool CheckoutStatusPartition::findImage(const char *image) {
		if (MTTable::empty() == true) {
			return false;
		}
		const MTSchema info(MTSchema::Text, DB_FILENAME);
		MTColumn column(info);
		column.set(image);
		if (MTTable::find(column) == false) {
			return false;
		}
		return true;
	}
	
	std::string CheckoutStatus::m_Master;
	std::string CheckoutStatus::m_workspace;
	std::string CheckoutStatus::m_primaryIndex;

	

	bool CheckoutStatus::Init(const char *Master, const char *workspace, const char *primaryIndex) {
		m_Master = Master;
		m_workspace = workspace;
		m_primaryIndex = primaryIndex;
		
		return true;
	}

	bool CheckoutStatus::newImage(const char *img, const char *comment) {
		std::string imagePath = img;
		PathController pathController(img);
		pathController.splitShort(img);
		CheckoutRow checkoutRow;

#ifdef WIN32
		checkoutRow.columnAt(DB_FILENAME) = pathController.getImage();
		checkoutRow.columnAt(DB_FILEPATH) = pathController.getYearday();
#else
		checkoutRow.columnAt(DB_FILENAME) = pathController.getImage().c_str();
		//tmp = pathController.getImage().c_str();
		checkoutRow.columnAt(DB_FILEPATH) = pathController.getYearday().c_str();
		//tmp = pathController.getYearday().c_str();
#endif
		checkoutRow.columnAt(DB_EVENT) = static_cast<int>(HistoryEvent::Event::ADDED);
		checkoutRow.columnAt(DB_VERSION) = 0;
		ExifDateTime dateAdded;
		dateAdded.now();
		checkoutRow.columnAt(DB_DATEADDED) = dateAdded;

		std::string pidxPath = m_primaryIndex;
		pidxPath += '/';
		pidxPath += pathController.getYear();
		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			if(SAUtils::mkDir(pidxPath.c_str()) == false) {
				return false;
			}
		}
		std::string workPath = m_workspace;
		workPath += '/';
		workPath += pathController.getYear();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			if (SAUtils::mkDir(workPath.c_str()) == false) {
				return false;
			}
		}
		workPath += '/';
		workPath += ".imga";
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			if (SAUtils::mkDir(workPath.c_str()) == false) {
				return false;
			}
		}
		CheckoutStatusPartition checkoutPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;
		
		if (SAUtils::FileExists(pidxPath.c_str()) == true) {
			checkoutPartition.read(pidxPath.c_str());
		}
		checkoutPartition.addRow(checkoutRow);	
		checkoutPartition.write(pidxPath.c_str());

		filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		workPath += "/";
		workPath += filenameStr;
		
		if (SAUtils::FileExists(workPath.c_str()) == true) {
			checkoutPartition.read(workPath.c_str());
		}
		checkoutPartition.addRow(checkoutRow);
		checkoutPartition.write(workPath.c_str());

		return true;
		
	}

	SharedMTRow CheckoutStatus::getRow(const char *img) {
		std::string imagePath = img;

		PathController pathController(m_primaryIndex.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return nullptr;
		}

		std::string pidxPath = m_primaryIndex.c_str();
		pidxPath += '/'; pidxPath += pathController.getYear();
		if (SAUtils::DirExists(pidxPath.c_str()) == false) {
			return nullptr;
		}


		std::string workPath = m_workspace.c_str();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::WORKSPACE_NOT_FOUND);
			return nullptr;
		}
		workPath += '/'; workPath += pathController.getYear();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return nullptr;
		}

		CheckoutStatusPartition checkoutPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;

		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::FILE_NOT_FOUND);
			return nullptr;
		}
		if (checkoutPartition.read(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
			return nullptr;
		}
		if (checkoutPartition.findImage(pathController.getImage().c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::IMAGE_NOT_FOUND);
			return nullptr;
		}
		SharedMTRow row = checkoutPartition.getCurrentRow();
		return row;
	}

	

	bool CheckoutStatus::checkInOutUpdate(const char *img, const HistoryEvent::Event& event, const char *comment) {
		std::string imagePath = img;

		PathController pathController(m_primaryIndex.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		std::string pidxPath = m_primaryIndex.c_str();
		pidxPath += '/'; pidxPath += pathController.getYear();
		if (SAUtils::DirExists(pidxPath.c_str()) == false) {
			return false;
		}


		std::string workPath = m_workspace.c_str();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::WORKSPACE_NOT_FOUND);
			return false;
		}
		workPath += '/'; workPath += pathController.getYear();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		CheckoutStatusPartition checkoutPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;

		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::FILE_NOT_FOUND);
			return false;
		}
		if (checkoutPartition.read(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
			return false;
		}
		if (checkoutPartition.findImage(pathController.getImage().c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::IMAGE_NOT_FOUND);
			return false;
		}
		SharedMTRow row = checkoutPartition.getCurrentRow();
		row->columnAt(DB_EVENT) = static_cast<int>(event);
		int version = row->columnAt(DB_VERSION).getInt();
		if (event == HistoryEvent::Event::CHECKIN) {
			row->columnAt(DB_VERSION) = ++version;
		}
		ExifDateTime dateAdded;
		dateAdded.now();
		row->columnAt(DB_DATEADDED) = dateAdded;

		if (checkoutPartition.write(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::WRITE_ERROR);
			return false;
		}
		return true;
	}

	bool CheckoutStatus::checkoutUpdate(const char *img, const char *comment) {
		return checkInOutUpdate(img, HistoryEvent::Event::CHECKOUT, comment);	
	}

	

	bool CheckoutStatus::checkinUpdate(const char *img, const char *comment) {
		
		return checkInOutUpdate(img, HistoryEvent::Event::CHECKIN, comment);
	}

	bool CheckoutStatus::uncheckoutUpdate(const char *img, const char *comment) {

		return checkInOutUpdate(img, HistoryEvent::Event::UNCHECKOUT, comment);
	}

	bool CheckoutStatus::isCheckedInOut(const char *img, bool in)
	{
		HistoryEvent::Event event = CheckoutStatus::checkoutStatus(img);	
		if (in) { // is checked in
			if (event == HistoryEvent::Event::CHECKIN) {
				return true;
			}
			if (event == HistoryEvent::Event::CHECKOUT) {
				ErrorCode::setErrorCode(IMGA_ERROR::ALREADY_CHECKED_OUT);
				return false;
			}
		}
		else {
			if (event == HistoryEvent::Event::CHECKOUT) {
				return true;
			}
			if (event == HistoryEvent::Event::CHECKIN) {
				ErrorCode::setErrorCode(IMGA_ERROR::ALREADY_CHECKED_IN);
				return false;
			}
			
		}
		return false;
	}

	bool CheckoutStatus::isCheckedIn(const char *img) {
		HistoryEvent::Event event = CheckoutStatus::checkoutStatus(img);
		// This funcion uses the last action to determine if the image is chected in or out.
		// These event indec
		if (event == HistoryEvent::Event::CHECKIN || event == HistoryEvent::Event::ADDED || event == HistoryEvent::Event::UNCHECKOUT) {
			return true;
		}
		if (event == HistoryEvent::Event::CHECKOUT) {
			ErrorCode::setErrorCode(IMGA_ERROR::ALREADY_CHECKED_OUT);
			return false;
		}
		ErrorCode::setErrorCode(IMGA_ERROR::ERROR);
		return false;
	}

	bool CheckoutStatus::isCheckedOut(const char *img) {
		HistoryEvent::Event event = CheckoutStatus::checkoutStatus(img);
		
		if (event == HistoryEvent::Event::CHECKOUT) {
			return true;
		}
		if (event == HistoryEvent::Event::CHECKIN) {
			ErrorCode::setErrorCode(IMGA_ERROR::ALREADY_CHECKED_IN);
			return false;
		}
		if (event == HistoryEvent::Event::UNCHECKOUT) {
			ErrorCode::setErrorCode(IMGA_ERROR::ALREADY_CHECKED_IN);
			return false;
		}
		if (event == HistoryEvent::Event::ADDED) {
			ErrorCode::setErrorCode(IMGA_ERROR::NOT_BEEN_CHECKED_OUT);
			return false;
		}
		return false;
	}

	bool CheckoutStatus::isPresent(const char * img)
	{
		PathController pathController(m_primaryIndex.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		std::string workPath = m_workspace.c_str();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::WORKSPACE_NOT_FOUND);
			return false;
		}
		workPath += '/'; workPath += pathController.getYear();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		std::string fullPath = workPath;
		fullPath += '/';
		fullPath += img;

		//unsigned long size = 0;

		if (SAUtils::FileExists(fullPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::IMAGE_NOT_FOUND);
			return false;
		}
		
		return true;;
	}

	bool CheckoutStatus::isChanged(const char * img, unsigned int versionCRC, const char *versionMD5)
	{
		PathController pathController(m_primaryIndex.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		std::string workPath = m_workspace.c_str();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::WORKSPACE_NOT_FOUND);
			return false;
		}
		workPath += '/'; workPath += pathController.getYear();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		std::string fullPath = workPath;
		fullPath += '/';
		fullPath += img;
		
		unsigned long size = 0;
		
		if (SAUtils::fileSize(fullPath.c_str(), &size) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		std::string buf;
		SAUtils::getFileContents(fullPath.c_str(), buf);
		MD5 md5(buf);
		std::string md5str = md5.hexdigest();
		
		CIDKCrc Crc;
		unsigned int crc = Crc.crc((unsigned char *)buf.c_str(), size);
		if (crc == versionCRC && (md5str.compare(versionMD5) == 0)) {
			ErrorCode::setErrorCode(IMGA_ERROR::NO_CHANGE_IN_IMAGE);
			return false;
		}
		return true;;
	}

	bool CheckoutStatusAction::onEnd()
	{
		/*
		if (!m_resultsList->write(ResultsList::FormatType::Human)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		*/
		return true;
	}

	bool CheckoutStatusAction::onImage(const char* name)
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
		m_resultsList->emplace_back(m_currentRow);
		return true;
	}

	bool CheckoutStatusAction::onStart()
	{
		m_resultsList = std::make_shared<ResultsList>(m_mtTableSchema);
		return true;
	};

	bool CheckoutStatusAction::onDayFolder(const char* name)
	{
		m_partition = std::make_shared<CheckoutStatusPartition>();

		if (m_partition->read(name) == false) {
			return false;
		}

		for (auto i = m_partition->begin(); i != m_partition->end(); i++) {
			std::shared_ptr<MTRow> row = *i;
			std::string path = row->columnAt(DB_FILEPATH).getString();
			path += '/';
			std::string image = row->columnAt(DB_FILENAME).getString();
			path += image;
			m_currentRow = row;
			if (!m_addressScope->isImageInScope(image.c_str())) {
				continue;
			}
			if (onImage(path.c_str()) == false) {
				return false;
			}
			
		}
		return true;
	}





	bool CheckoutStatus::scopedStatus(const char * scope)
	{

		/*
		CheckoutTableIndex checkoutTableIndex(std::make_shared<StatusAction>());
		//checkoutTableIndex.setIndexPath(m_primaryIndex.c_str());
		if (!checkoutTableIndex.setScope(scope)) {
			return false;
		}
		checkoutTableIndex.process(m_primaryIndex.c_str());
		*/
		return true;
	}

	bool CheckoutStatus::showCheckedOut(const char *addressScope, ResultsPresentation::FormatType formatType, const char* file) {
		CLogger& logger = CLogger::getLogger();
		setPath(m_primaryIndex.c_str());
		if (select(addressScope) == false) {
			logger.log(LOG_OK, CLogger::Level::ERR, "Cannot process archive history");
			return false;
		}
		std::shared_ptr<ResultsList> results = getResults();
		if (results == nullptr) {
			logger.log(LOG_OK, CLogger::Level::WARNING, "No results for archive history");
			return false;
		}
		std::string fileStr = file;
		CheckoutStatusResultsPresentation resultsPresentation(*results);
		if (!fileStr.empty()) {
			resultsPresentation.setFilename(file);
		}
		resultsPresentation.write(formatType);
		return true;
	}

	bool CheckoutStatus::showUncheckedOutChanges(const char *addressScope, ResultsPresentation::FormatType formatType, const char* file) {
		if (addressScope == nullptr) {
			// All images

		}
		//FolderList folderList(m_Master.c_str(), m_workspace.c_str());
		//folderList.showUncheckedOutChanges(addressScope);

		return true;
	}

	bool CheckoutStatus::add(CheckoutRow &checkoutRow, const char *img) {
		// check path exists
		if (SAUtils::DirExists(m_primaryIndex.c_str()) == false) {
			if (SAUtils::mkDir(m_primaryIndex.c_str()) == false) {
				throw std::exception();
			}
		}
		std::string checkoutStatusFileRoot = m_primaryIndex;
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


		CheckoutStatusPartition checkoutPartition;
		std::string filename = checkoutRow.getSchema().getName() + ".csv";
		if (checkoutPartition.read(chkoutPath.c_str(), filename.c_str()) == false) {
			if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
				// file may not exist
				return false;
			}
		}
		if (checkoutPartition.addRow(checkoutRow) == false) {
			return false;
		}
		if (checkoutPartition.write(chkoutPath.c_str(), filename.c_str()) == false) {
			return false;
		}
		return true;
		//m_mirrorDB->process(relPath);
	}

	


	const HistoryEvent::Event CheckoutStatus::checkoutStatus(const char *img) {
		std::string imagePath = img;

		PathController pathController(m_primaryIndex.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::TARGET_NOT_FOUND);
			return HistoryEvent::Event::ERROR;
		}

		std::string pidxPath = m_primaryIndex.c_str();
		pidxPath += '/'; pidxPath += pathController.getYear();
		if (SAUtils::DirExists(pidxPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::TARGET_NOT_FOUND);
			return HistoryEvent::Event::ERROR;
		}

		/* This function is handling the csv database
		std::string workPath = m_workspace.c_str();
		workPath += '/'; workPath += pathController.getYear();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::WORKSPACE_NOT_FOUND);
			return HistoryEvent::Event::ERROR;
		}
		*/

		CheckoutStatusPartition checkoutPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;

		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			return HistoryEvent::Event::ERROR;
		}
		if (checkoutPartition.read(pidxPath.c_str()) == false) {
			return HistoryEvent::Event::ERROR;
		}
		if (checkoutPartition.findImage(pathController.getImage().c_str()) == false) {
			return HistoryEvent::Event::ERROR;
		}
		SharedMTRow row = checkoutPartition.getCurrentRow();
		HistoryEvent::Event event = static_cast<HistoryEvent::Event>(row->columnAt(DB_EVENT).getInt());
		//int version = row->columnAt(DB_VERSION).getInt();
		//row->columnAt(DB_VERSION) = ++version;
		//ExifDateTime dateAdded;
		//dateAdded.now();
		//row->columnAt(DB_DATEADDED) = dateAdded;
		return event;

		
	}
	
	/*
	bool CheckoutStatus::status(const char *scope) {

		
		IndexVisitor IndexVisitor(new StatusAction);
		IndexVisitor.process("C:/ProgramData/IDK-Software/SIA/pi/chkout");
		


		AddressScope addressScope;
		std::string scopeStr = scope;

		if (scopeStr.empty()) {
			addressScope.scopeAll();
		}
		else {
			if (!addressScope.scope(scope)) {
				return false;
			}
		}
		
		std::string dataPath = m_primaryIndex;
		std::shared_ptr<CheckoutStatusLog> log = std::make_shared<CheckoutStatusLog>();
		std::string rootPath = m_primaryIndex;
		FileList_Ptr yearlist = SAUtils::getFiles_(rootPath.c_str());
		for (std::vector<std::string>::iterator i = yearlist->begin(); i != yearlist->end(); i++) {
			std::string dataString = *i;
			if (dataString[0] == '.') {
				continue;
			}

			std::string year = dataString.substr(0, 4);

			if (year.compare("system") == 0) {
				continue;
			}
			//printf("Year found %s\n", year.c_str());
			std::string yearPath = dataPath += '/';
			yearPath += year;

			if (!addressScope.isInScope(year.c_str())) {
				continue;
			}

			FileList_Ptr daylist = SAUtils::getFiles_(yearPath.c_str());
			for (std::vector<std::string>::iterator i = daylist->begin(); i != daylist->end(); i++) {

				std::string dataString = *i;
				if (dataString[0] == '.') {
					continue;
				}
				std::string dateStr = dataString.substr(0, 10);
				if (!addressScope.isInScope(dateStr.c_str())) {
					continue;
				}
				std::string filepath = yearPath + "/" + dataString;

				printf("%s\n", filepath.c_str());
				processFile(log, filepath);
			}

		}
		
		if (!log->write(LogDocument::FormatType::Human)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}

	bool CheckoutStatus::processFile(std::shared_ptr<CheckoutStatusLog> log, const std::string yyyymmddFile) {
		CheckoutPartition checkoutPartition;
		if (checkoutPartition.read(yyyymmddFile.c_str()) == false) {
			if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
				// file may not exist
				return false;
			}
		}
		
		for (auto i = checkoutPartition.begin(); i != checkoutPartition.end(); i++) {
			std::string s;
			s = (*i)->toString();
			log->push_back(s);
			//printf("%s\n", (*i)->toString().c_str());
		}
		
		return true;
	}
	*/
	
	/*
	std::shared_ptr<CheckoutStatusLog>  CheckoutStatus::getEntries(const char *filepath) {

		std::shared_ptr<CheckoutStatusLog> log = std::make_shared<CheckoutStatusLog>();

		CheckoutPartition checkoutPartition;
		if (checkoutPartition.read(filepath) == false) {
			if (ErrorCode::getErrorCode() != IMGA_ERROR::OPEN_ERROR) {
				// file may not exist
				return false;
			}
		}
		//log->setTitle(indexController.getImageName().c_str());
		log->setDescription(filepath);
		return log;
	}


	bool CheckoutStatus::historyLog(const char *filepath, LogDocument::FormatType formatType) {
		std::shared_ptr<CheckoutStatusLog> log;
		//if ((log = imageHistory->getEntries(filepath)) == nullptr) {
		//	ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
		//	return false;
		//}

		if (!log->write(formatType)) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}
	*/

/*
std::string CheckoutTableIndex::m_primaryIndex;

bool CheckoutTableIndex::Init(const char* primaryIndex) {
	m_primaryIndex = primaryIndex;

	return true;
}

bool CheckoutTableIndex::process() {
	if (!CheckoutTableIndex::process(m_primaryIndex.c_str())) {
		return false;
	}
	return true;
}


bool CheckoutTableIndex::process(const char* rootFolder) {
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
		catch (std::exception) {
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

			m_indexAction->onDayEnd();
		}
		m_indexAction->onYearEnd();

	}
	m_indexAction->onEnd();
	return true;
}

bool onDayFolder(const char* name)
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

bool StatusAction::onDayFolder(const char* name)
{
	m_currentPartition = std::make_shared<CheckoutPartition>();

	if (m_currentPartition->read(name) == false) {
		return false;
	}

	for (auto i = m_currentPartition->begin(); i != m_currentPartition->end(); i++) {
		std::shared_ptr<MTRow> row = *i;

		m_currentRow = std::static_pointer_cast<CheckoutRow>(row);
		const char* fileName = m_currentRow->getFileName();
		if (!m_addressScope->isImageInScope(fileName)) {
			continue;
		}
		if (onImage() == false) {
			return false;
		}

	}

	return false;
}

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

*/


/*
	CheckoutStatusLog::CheckoutStatusLog() : LogDocument(std::make_shared<CheckoutSchema>()) {
		// TODO Auto-generated constructor stub

	}

	CheckoutStatusLog::~CheckoutStatusLog() {
		// TODO Auto-generated destructor stub
	}

	bool CheckoutStatusLog::writeHuman() {
		
		std::cout << "\nStatus Date        Time             version Image Comment\n\n";
		std::string addr;
		bool first = true;
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			//std::cout << *i << '\n';
			CSVArgs csvArgs(':');
			if (csvArgs.process(i->c_str()) == false) {
				return false;
			}
			if (!first && addr.compare(csvArgs.at(1)) != 0) {
				
				std::cout << "---------------------------------------------------\n";
				
			}
			addr = csvArgs.at(1);
			first = false;
			std::string status = csvArgs.at(2);
			
			int value = std::stoi(status);
			HistoryEvent::Event val = static_cast<HistoryEvent::Event>(value);
			std::cout << HistoryEvent::getChar(val) << "      ";
			std::cout << csvArgs.at(1) << "      ";
			std::cout << csvArgs.at(4) << "  ";
			std::cout << csvArgs.at(3) << "  ";
			std::cout << csvArgs.at(0) << '\n';
			
		}
		return true;
	}

	bool CheckoutStatusLog::writeJson() {
		return true;
	}

	bool CheckoutStatusLog::writeCSV() {
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			std::cout << *i << '\n';
		}
		return true;
	}

	bool CheckoutStatusLog::writeXML() {
		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			std::cout << *i << '\n';
			CSVArgs csvArgs(':');
			if (csvArgs.process(i->c_str()) == false) {
				return false;
			}
			std::cout << "\t<Event>\n";
			std::cout << writeTag("DateTime", csvArgs.at(0), 2);
			std::cout << writeTag("ImagePage", csvArgs.at(1), 2);
			std::cout << writeTag("Value", csvArgs.at(2), 2);
			std::cout << writeTag("Comment", csvArgs.at(3), 2);
			std::cout << writeTag("Event", csvArgs.at(4), 2);
			std::cout << "\t</Event>\n";
		}
		std::cout << "</Catalog>\n";
		return true;
	}
	*/



}
