#include <string>
#include "CheckoutStatus.h"
#include "CheckDisk.h"
#include "FolderList.h"
#include "pathcontroller.h"
#include "FileInfo.h"
#include "DBDefines.h"
#include "MetaType.h"



namespace simplearchive {

	class CheckoutSchema : public MTTableSchema {
	public:
		CheckoutSchema() : MTTableSchema("CheckoutProperties") {
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Text, DB_EVENT));
			add(MTSchema(MTSchema::Text, DB_VERSION));
		}
	};

	class CheckoutRow : public MTRow {
		static CheckoutSchema m_tableSchema;
		friend class MetadataTemplate;
	public:
		CheckoutRow();
		CheckoutRow(const MTRow &row) : MTRow(m_tableSchema) {

			for (unsigned int i = 0; i < row.size(); i++) {
				MTColumn& c1 = *at(i);
				MTColumn& c2 = *row.at(i);
				c1 = c2;
			}

		}
	};

	class CheckoutPartition : public MTTable {
	public:
		CheckoutPartition() : MTTable(new CheckoutSchema) {};
		virtual ~CheckoutPartition() {};
	};

	
	std::string CheckoutStatus::m_Master;
	std::string CheckoutStatus::m_workspace;
	std::string CheckoutStatus::m_primaryIndex;


	CheckoutStatus::CheckoutStatus()
	{
		m_LastError = Status::Ok;
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
		return true;
	}

	bool CheckoutStatus::checkout(const char *img) {
		std::string imagePath = img;

		PathController pathController(m_Master.c_str());
		pathController.splitShort(img);
		if (pathController.makePath(false) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
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

		// CheckoutStatusFile
		
		
		

		/* checks againt the first version only (incorrect)
		unsigned int acrc = checkDisk.getCrc(path.c_str(), pathController.getImage().c_str());
		pathController.makeRelativeImagePath(img);
		std::string workspace = m_workspace;
		
		workspace += pathController.getShortRelativePath();

		PathController workspacePath(workspace.c_str(), false);
		workspacePath.split();
		workspacePath.makePath(false);
		if (PathController::doValidate(workspacePath.getFullPath().c_str()) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::TARGET_INVALID_PATH);
			return false;
		}
		
		if (PathController::doValidate(workspace.c_str()) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::TARGET_NOT_FOUND);
			return false;
		}
		
		FileInfo fileInfo(workspace);
		
		if (fileInfo.getCrc() != acrc) {
			ErrorCode::setErrorCode(SIA_ERROR::WILL_OVERWRITE_CHANGES);
			return false;
		}
		if (checkDisk.checkout(path.c_str(), pathController.getImage().c_str()) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}
		*/
		return true;
	}

	bool CheckoutStatus::checkin(const char *img) {
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
		return true;
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