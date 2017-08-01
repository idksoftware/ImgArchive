#include <string>
#include "CheckoutStatus.h"
#include "CheckDisk.h"
#include "FolderList.h"
#include "pathcontroller.h"
#include "FileInfo.h"

namespace simplearchive {

	std::string CheckoutStatus::m_Master;
	std::string CheckoutStatus::m_workspace;

	CheckoutStatus::CheckoutStatus()
	{
		m_LastError = Status::Ok;
	}


	CheckoutStatus::~CheckoutStatus()
	{
	}

	bool CheckoutStatus::Init(const char *Master, const char *workspace) {
		m_Master = Master;
		m_workspace = workspace;
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
#ifdef TESTING		
		if (checkDisk.checkout(path.c_str(), pathController.getImage().c_str()) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}
#endif			
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
}