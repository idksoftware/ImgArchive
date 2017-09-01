#include <string>
#include "CheckoutStatus.h"
#include "CheckDisk.h"
#include "FolderList.h"
#include "pathcontroller.h"
#include "FileInfo.h"
#include "DBDefines.h"
#include "MetaType.h"
#include "HistoryEvent.h"
#include "ErrorCode.h"


namespace simplearchive {

	class CheckoutSchema : public MTTableSchema {
	public:
		CheckoutSchema() : MTTableSchema("CheckoutProperties") {
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Integer, DB_EVENT));
			add(MTSchema(MTSchema::Integer, DB_VERSION));
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
		}
	};

	class CheckoutRow : public MTRow {
		static CheckoutSchema m_tableSchema;
		friend class MetadataTemplate;
	public:
		CheckoutRow() : MTRow(m_tableSchema) {};
		CheckoutRow(const MTRow &row) : MTRow(m_tableSchema) {

			for (unsigned int i = 0; i < row.size(); i++) {
				MTColumn& c1 = *at(i);
				MTColumn& c2 = *row.at(i);
				c1 = c2;
			}

		}
	};

	CheckoutSchema CheckoutRow::m_tableSchema;

	class CheckoutPartition : public MTTable {
	public:
		CheckoutPartition() : MTTable(new CheckoutSchema) {};
		virtual ~CheckoutPartition() {};
		bool findImage(const char *image);
	};

	bool CheckoutPartition::findImage(const char *image) {
		const MTSchema info(MTSchema::Text, DB_FILENAME);
		MTColumn column(info);
		column.set(image);
		if (find(column) == false) {
			return false;
		}
		return true;
	}

	
	std::string CheckoutStatus::m_Master;
	std::string CheckoutStatus::m_workspace;
	std::string CheckoutStatus::m_primaryIndex;

	CheckoutStatus::CheckoutStatus()
	{
	}

	CheckoutStatus::~CheckoutStatus()
	{
	}

	bool CheckoutStatus::Init(const char *Master, const char *workspace, const char *primaryIndex) {
		m_Master = Master;
		m_workspace = workspace;
		m_primaryIndex = primaryIndex;
		return true;
	}

	bool CheckoutStatus::newImage(const char *img) {
		std::string imagePath = img;
		PathController pathController(img);
		pathController.splitShort(img);
		CheckoutRow checkoutRow;
		checkoutRow.columnAt(DB_FILENAME) = pathController.getImage();
		checkoutRow.columnAt(DB_EVENT) = static_cast<int>(HistoryEvent::ADDED);
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
		workPath += ".sia";
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			if (SAUtils::mkDir(workPath.c_str()) == false) {
				return false;
			}
		}
		CheckoutPartition checkoutPartition;
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

	bool CheckoutStatus::checkInOut(const char *img, const HistoryEvent::Event& event) {
		std::string imagePath = img;

		PathController pathController(m_Master.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			return false;
		}

		std::string pidxPath = m_primaryIndex.c_str();
		pidxPath += '/'; pidxPath += pathController.getYear();
		if (SAUtils::DirExists(pidxPath.c_str()) == false) {
			return false;
		}


		std::string workPath = m_workspace.c_str();
		workPath += '/'; workPath += pathController.getYear();
		if (SAUtils::FileExists(workPath.c_str()) == false) {
			return false;
		}

		CheckoutPartition checkoutPartition;
		std::string filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		pidxPath += "/";
		pidxPath += filenameStr;

		if (SAUtils::FileExists(pidxPath.c_str()) == false) {
			return false;
		}
		if (checkoutPartition.read(pidxPath.c_str()) == false) {
			return false;
		}
		if (checkoutPartition.findImage(pathController.getImage().c_str()) == false) {
			return false;
		}
		SharedMTRow row = checkoutPartition.getCurrentRow();
		row->columnAt(DB_EVENT) = static_cast<int>(HistoryEvent::CHECKOUT);
		int version = row->columnAt(DB_VERSION).getInt();
		row->columnAt(DB_VERSION) = ++version;
		ExifDateTime dateAdded;
		dateAdded.now();
		row->columnAt(DB_DATEADDED) = dateAdded;

		if (checkoutPartition.write(pidxPath.c_str()) == false) {
			return false;
		}


		filenameStr = pathController.getYearday();
		filenameStr += ".csv";
		workPath += "/.sia/";
		workPath += filenameStr;

		if (SAUtils::FileExists(workPath.c_str()) == false) {
			return false;
		}
		if (checkoutPartition.read(workPath.c_str()) == false) {
			return false;
		}
		if (checkoutPartition.findImage(pathController.getImage().c_str()) == false) {
			return false;
		}
		row = checkoutPartition.getCurrentRow();
		row->columnAt(DB_EVENT) = static_cast<int>(event);
		version = row->columnAt(DB_VERSION).getInt();
		row->columnAt(DB_VERSION) = ++version;

		dateAdded.now();
		row->columnAt(DB_DATEADDED) = dateAdded;

		if (checkoutPartition.write(workPath.c_str()) == false) {
			return false;
		}

		return true;
	}

	bool CheckoutStatus::checkout(const char *img) {
		std::string imagePath = img;

		PathController pathController(m_Master.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			return false;
		}
		
		std::string path = pathController.getFullPath();
		path += "/chdsk/fdata.csv";

		CheckDisk checkDisk;
		if (checkDisk.ischeckedOut(path.c_str(), pathController.getImage().c_str()) == false) {
			if (ErrorCode::getErrorCode() == SIA_ERROR::IMAGE_NOT_FOUND) {
				return false; // error code set
			}
			// returned false meaning it is not already checked out and is not an error. 
		}
		else {
			ErrorCode::setErrorCode(SIA_ERROR::ALREADY_CHECKED_OUT);
			return false;
		}

		return checkInOut(img, HistoryEvent::CHECKOUT);
		
	}

	bool CheckoutStatus::checkin(const char *img) {
		/*
		std::string imagePath = img;

		PathController pathController(m_Master.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			return false;
		}

		std::string path = pathController.getFullPath();
		path += "/chdsk/fdata.csv";

		CheckDisk checkDisk;
		if (checkDisk.checkin(path.c_str(), pathController.getImage().c_str()) == false) {
			if (ErrorCode::getErrorCode() == SIA_ERROR::IMAGE_NOT_FOUND) {
				return false; // error code set
			}
			// returned false meaning it is not already checked out and is not an error.
			ErrorCode::setErrorCode(SIA_ERROR::ALREADY_CHECKED_IN);
			return false;
		}
		*/
		return checkInOut(img, HistoryEvent::CHECKIN);
	}

	bool CheckoutStatus::showCheckedOut(const char *addressScope) {
		if (addressScope == nullptr) {
			// All images

		}
		FolderList folderList(m_Master.c_str());
		folderList.showCheckedOut(addressScope);

		return true;
	}

	bool CheckoutStatus::showUncheckedOutChanges(const char *addressScope) {
		if (addressScope == nullptr) {
			// All images

		}
		FolderList folderList(m_Master.c_str(), m_workspace.c_str());
		folderList.showUncheckedOutChanges(addressScope);

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


		CheckoutPartition checkoutPartition;
		std::string filename = checkoutRow.getSchema().getName() + ".csv";
		if (checkoutPartition.read(chkoutPath.c_str(), filename.c_str()) == false) {
			if (ErrorCode::getErrorCode() != SIA_ERROR::OPEN_ERROR) {
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
}