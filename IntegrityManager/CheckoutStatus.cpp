#include <string>
#include "CheckoutStatus.h"
#include "CheckDisk.h"
#include "FolderList.h"

namespace simplearchive {

	std::string CheckoutStatus::m_shadow;
	std::string CheckoutStatus::m_workspace;

	CheckoutStatus::CheckoutStatus()
	{
	}


	CheckoutStatus::~CheckoutStatus()
	{
	}

	bool CheckoutStatus::Init(const char *shadow, const char *workspace) {
		m_shadow = shadow;
		m_workspace = workspace;
		return true;
	}

	bool CheckoutStatus::checkout(const char *img) {
		std::string imagePath = img;

		std::string year = imagePath.substr(0, 4);
		std::string archiveImagePath = m_shadow;
		archiveImagePath += '/'; archiveImagePath += year;
		std::string yearday = imagePath.substr(0, 10);
		archiveImagePath += '/'; archiveImagePath += yearday;
		archiveImagePath += "/chdsk/fdata.csv";

		std::string image = imagePath.substr(11, imagePath.length() - 11);

		CheckDisk checkDisk;
		if (checkDisk.checkout(archiveImagePath.c_str(), image.c_str()) == false) {
			return false;
		}
			/*
			std::string workspaceImagePath = m_workspacePath;
			workspaceImagePath += '/'; workspaceImagePath += year;
			workspaceImagePath += '/'; workspaceImagePath += imagePath;
			*/
		return true;
	}

	bool CheckoutStatus::checkin(const char *img) {
		std::string imagePath = img;

		std::string year = imagePath.substr(0, 4);
		std::string archiveImagePath = m_shadow;
		archiveImagePath += '/'; archiveImagePath += year;
		std::string yearday = imagePath.substr(0, 10);
		archiveImagePath += '/'; archiveImagePath += yearday;
		archiveImagePath += "/chdsk/fdata.csv";

		std::string image = imagePath.substr(11, imagePath.length() - 11);

		CheckDisk checkDisk;
		if (checkDisk.checkin(archiveImagePath.c_str(), image.c_str()) == false) {
			return false;
		}
		/*
		std::string workspaceImagePath = m_workspacePath;
		workspaceImagePath += '/'; workspaceImagePath += year;
		workspaceImagePath += '/'; workspaceImagePath += imagePath;
		*/
		return true;
	}

	bool CheckoutStatus::showCheckedOut(const char *addressScope) {
		if (addressScope == nullptr) {
			// All images

		}
		FolderList folderList(m_shadow.c_str());
		folderList.showCheckedOut(addressScope);

		return true;
	}

	bool CheckoutStatus::showUncheckedOutChanges(const char *addressScope) {
		if (addressScope == nullptr) {
			// All images

		}
		FolderList folderList(m_shadow.c_str(), m_workspace.c_str());
		folderList.showUncheckedOutChanges(addressScope);

		return true;
	}
}