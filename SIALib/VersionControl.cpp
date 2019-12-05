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

#include <string.h>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <stdlib.h>

#include "CSVDBFile.h"

#include "ImageContainer.h"
#include "HistoryEvent.h"
#include "History.h"
#include "CLogger.h"
#include "Version.h"
#include "CDate.h"
#include "SAUtils.h"
#include "ErrorCode.h"

#include "pathcontroller.h"
#include "CSVDatabase.h"
#include "FolderList.h"
#include "IntegrityManager.h"
#include "VersionMetadataObject.h"
#include "CheckoutStatus.h"
#include "MetadataObject.h"
#include "SummaryFile.h"
#include "CheckDiskJournal.h"
#include "VersionControl.h"
#include "XMLWriter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#undef FILECODE
#define FILECODE VERSIONCONTROL_CPP
#define LOG_COMPLETED_OK						VERSIONCONTROL_CPP 
#define LOG_CHECKOUT_FUNCTION					(LOG_COMPLETED_OK + 1)
#define LOG_UNABLE_TO_CHECKOUT_GENERAL			(LOG_CHECKOUT_FUNCTION + 1)
#define LOG_UNABLE_TO_CHECKIN_GENERAL			(LOG_UNABLE_TO_CHECKOUT_GENERAL + 1)
#define LOG_UNCHECKOUT_FUNCTION					(LOG_UNABLE_TO_CHECKIN_GENERAL + 1)
#define LOG_UNABLE_TO_UNCHECKOUT_GENERAL		(LOG_UNCHECKOUT_FUNCTION + 1)


namespace simplearchive {

std::string VersionControl::m_pathToPrimary;
std::string VersionControl::m_pathToMaster;
std::string VersionControl::m_pathToDerivative;
std::string VersionControl::m_workspace;

void VersionControl::setPaths(const char *primary, const char *master, const char *derivative, const char *workspace) {
	m_pathToPrimary = primary;
	m_pathToMaster = master;
	m_pathToDerivative = derivative;
	m_workspace = workspace;
}

VersionControl &VersionControl::getInstance() {
	static VersionControl versionControl;
	return versionControl;
}

bool VersionControl::setCurrentVersion(const char *filepath) {
	CLogger &logger = CLogger::getLogger();
	PathController pathController;
	if (pathController.splitShort(filepath) == false) {
		return false;
	}
	std::string fullWS = m_workspace;
	fullWS += '/';
	fullWS += pathController.getYear();
	fullWS += '/';
	fullWS += filepath;

	// Note the file in the workspace may not exist
	//if (!SAUtils::FileExists(fullWS.c_str())) {
	//	return false;
	//}
	m_relativeWorkspacePath = filepath;
	if (m_versionIndex->setRowCursor(filepath) == false) {
		//logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
		return false;
	}
	SharedMTRow row = m_versionIndex->getCurrentRow();
	m_version = row->columnAt(DB_VERSION).getInt();
	m_currentVersionPath = row->columnAt(DB_VERSIONPATH).getString();
	m_crc = row->columnAt(DB_CRC).getInt();
	m_md5 = row->columnAt(DB_MD5).getString();
	return true;
}

void VersionControl::setCurrentVersion(const char *repositoryPath, const char *sourceRelPath, const char *fileName, int version) {
	m_currentVersion = std::make_shared<Version>(repositoryPath, sourceRelPath, fileName, version);
}

bool VersionControl::createDerivativeVersion(FileInfo& fileInfo, int derivativeSeqNumber, int primarySeqNumber) {
	
	VersionMetadataObject vmo;
	PathController pathController(fileInfo.getPath().c_str());
	if (pathController.split() == false) {
		return false;
	}
	ExifDate addDate;
	vmo.columnAt(DB_SEQUENCEID) = primarySeqNumber;
	vmo.columnAt(DB_VERSION) = m_currentVersion->getVersion();
	vmo.columnAt(DB_DATABASEID) = derivativeSeqNumber;
	vmo.columnAt(DB_FILENAME) = m_currentVersion->getVersionName().c_str();
	vmo.columnAt(DB_ORGINALNAME) = fileInfo.getName().c_str();
	std::string shortRelPath = m_currentVersion->getShortRelPath();
	shortRelPath += '/';
	shortRelPath += m_currentVersion->makeEncodedVersion();
	vmo.columnAt(DB_VERSIONPATH) = shortRelPath.c_str();
	//vmo.columnAt(DB_VERSIONPATH) = version.getVersionPath().c_str();
	vmo.columnAt(DB_CRC) = fileInfo.getCrc();
	vmo.columnAt(DB_MD5) = fileInfo.getMd5().c_str();
	vmo.columnAt(DB_UUID) = fileInfo.getUuid().c_str();
	vmo.columnAt(DB_FILESIZE) = fileInfo.getSize();
	vmo.columnAt(DB_DATEMODIFIED) = fileInfo.getModTime();
	vmo.columnAt(DB_DATECREATE) = fileInfo.getCreateTime();
	ExifDateTime addTime;

	vmo.columnAt(DB_DATEADDED) = addTime;

	if (m_versionIndex->createDerivativeVersion(vmo, pathController.getYearday().c_str(), vmo.columnAt(DB_DATABASEID).getInt(), vmo.columnAt(DB_SEQUENCEID).getInt(), fileInfo.getName().c_str()) == false) {
		return false;
	}
	return true;
}

bool VersionControl::checkoutFile(const char *filepath, const char *comment, bool force) {
	CLogger &logger = CLogger::getLogger();
	
	
	std::string targetRootPath = m_workspace;
	PathController pathController;
	if (pathController.splitShort(filepath) == false) {
		ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
		logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
		return false;
	}
	pathController.makeImagePath();
	if (setCurrentVersion(filepath) == false) {
		logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
		return false;
	}
	std::string versionPath = getCurrentVersionPath();
	int version = getVersion();

	CheckoutStatus checkoutStatus;
	// It should be checked in so to be checked out.
	if (checkoutStatus.isCheckedIn(filepath) == false) {

		if (ErrorCode::getErrorCode() == SIA_ERROR::ALREADY_CHECKED_OUT && !force) {
			// return after not checkinh out as checked out already and not forced to check out
			return false;
		}
		if (ErrorCode::getErrorCode() == SIA_ERROR::TARGET_NOT_FOUND) {
			// Not an error but the image needs to be copied form the repository to the workspace
			return false;
		}

		else if (ErrorCode::getErrorCode() == SIA_ERROR::WILL_OVERWRITE_CHANGES) {
			// This may be an option

			return false;
		}
		else {
			logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::INFO, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}

		return false;
	}
	if (getVersion() == 0) {
		// Original
		if (checkoutFromMaster(filepath, comment, force) == false) {
			if (ErrorCode::getErrorCode() != SIA_ERROR::CHANGE_MAY_BE_LOST) {
				logger.log((LOG_OK + 1), CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}
			// if changes may be lost then not copied but need checkin out.
		}
		
	}
	else {
		if (checkoutFromDerivatives(filepath, comment, force) == false) {

			if (ErrorCode::getErrorCode() == SIA_ERROR::CHANGE_MAY_BE_LOST && !force) {
				logger.log(LOG_OK, CLogger::Level::INFO, "checked out but image not copied\"%s\" Error: %s?", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				// check out;
			}
			else if (ErrorCode::getErrorCode() == SIA_ERROR::NO_CHANGE_IN_IMAGE && !force) {
				logger.log(LOG_OK, CLogger::Level::INFO, "image not copied \"%s\"? Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				// check out;
			}
			else {
				logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}


		}
	}

	

	if (checkoutStatus.checkoutUpdate(filepath, comment) == false) {
		if (ErrorCode::getErrorCode() == SIA_ERROR::ALREADY_CHECKED_OUT && !force) {
			// return after not checkinh out as checked out already and not forced to check out
			return false;
		}
		if (ErrorCode::getErrorCode() == SIA_ERROR::TARGET_NOT_FOUND) {
			// Not an error but the image needs to be copied form the repository to the workspace
			return false;
		}

		else if (ErrorCode::getErrorCode() == SIA_ERROR::WILL_OVERWRITE_CHANGES) {
			// This may be an option
			logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::INFO, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		else {
			logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
	}
	return true;
}

bool VersionControl::checkin(const char * relWorkspace, const char * relDb, bool force)
{
	if (makeVersionsList(relWorkspace) == false) {
		return false;
	}
	return false;
}

bool VersionControl::checkout(const char *relWorkspace, const char *relDb, bool force) {


	
	std::string from = m_pathToMaster;
	from += '/';
	from += relDb;

	std::string to = m_pathToMaster;
	to += '/';
	to += relWorkspace;

	CLogger &logger = CLogger::getLogger();

	if (!force) {
		if (SAUtils::FileExists(to.c_str()) == true) {

			FileInfo fileInfoFrom(from);
			FileInfo fileInfoTo(to);
			if (fileInfoFrom.getCrc() != fileInfoTo.getCrc()) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Changes may be lost by Checkout \"%s\"?", to.c_str());
				return false;
			}
			logger.log(LOG_OK, CLogger::Level::INFO, "Target image is the same as checked in image: \"%s\"?", from.c_str());
			return true;
		}
	}

	if (SAUtils::FileExists(from.c_str()) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", from.c_str());
		return false;
	}

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to copy version to destination: \"%s\"?", to.c_str());
		return false;
	}
	
	return true;
}

bool VersionControl::checkoutFromMaster(const char *targetRelPath, const char *comment, bool force) {
	CLogger &logger = CLogger::getLogger();


	PathController pathController(m_pathToMaster.c_str());
	pathController.splitPathAndFile(targetRelPath);

	if (pathController.makeRelativePath(targetRelPath) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid version path: \"%s\"?", targetRelPath);
		ErrorCode::setErrorCode(SIA_ERROR::IMAGE_NOT_FOUND);
		return false;
	}
	std::string from = m_pathToMaster.c_str();



	from += pathController.getRelativePath();
	from += "/images/";
	from += pathController.getImageName();
	pathController.setFullPath(from);
	if (pathController.isValid() == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", from.c_str());
		return false;
	}
	if (pathController.makeRelativePath(targetRelPath) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid target path: \"%s\"?", from.c_str());
		return false;
	}
	std::string to = m_workspace;
	to += '/';
	to += pathController.getRelativePath();
	to += '/';
	to += pathController.getImage();

	if (!force) {
		if (SAUtils::FileExists(to.c_str()) == true) {

			FileInfo fileInfoFrom(from);
			FileInfo fileInfoTo(to);
			if (fileInfoFrom.getCrc() != fileInfoTo.getCrc()) {
				logger.log(LOG_OK, CLogger::Level::WARNING, "Changes may be lost by Checkout \"%s\"?", targetRelPath);
				ErrorCode::setErrorCode(SIA_ERROR::CHANGE_MAY_BE_LOST);
				return false;
			}
			logger.log(LOG_OK, CLogger::Level::INFO, "Target image is the same as checked in image: \"%s\"?", from.c_str());
			return true;
		}
	}

	if (SAUtils::FileExists(from.c_str()) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", from.c_str());
		return false;
	}

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to copy version to destination: \"%s\"?", to.c_str());
		return false;
	}

	return true;
}

bool VersionControl::checkoutFromDerivatives(const char *targetRelPath, const char *comment, bool force) {
	
	CLogger &logger = CLogger::getLogger();
	std::string root = m_pathToDerivative.c_str();
	
	PathController pathController(root.c_str());
	pathController.splitShort(getCurrentVersionPath().c_str());
	if (pathController.makeImagePath()) {
		return false;
	}
	std::string derivativePath = root;
	derivativePath += '/';
	derivativePath += pathController.getYear();
	derivativePath += '/';
	derivativePath += pathController.getYearday();
	derivativePath += "/images/";
	derivativePath += pathController.getImage();
	
	/*
	Version version(root.c_str(), pathController.getYearday().c_str(), pathController.getImageName().c_str(), getVersion());
	std::string versionedFilename = version.makeEncodedVersion();
	if (pathController.makeRelativePath(m_currentVersionPath.c_str()) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid version path: \"%s\"?", m_currentVersionPath.c_str());
		return false;
	}
	std::string from = root.c_str();



	from += pathController.getRelativePath();
	from += "/images/";
	from += versionedFilename;
	pathController.setFullPath(from);
	*/
	if (SAUtils::FileExists(derivativePath.c_str()) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", derivativePath.c_str());
		return false;
	}
	//if (pathController.makeRelativePath(targetRelPath) == false) {
	//	logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid target path: \"%s\"?", from.c_str());
	//	return false;
	//}m_workspace
	PathController workspacePathControl(m_workspace.c_str());
	workspacePathControl.splitShort(targetRelPath);
	std::string to = m_workspace;
	to += '/';
	to += workspacePathControl.getYear();
	to += '/';
	to += targetRelPath;

	//if (SAUtils::FileExists(from.c_str()) == false) {
	//	logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", from.c_str());
	//	return false;
	//}

	if (SAUtils::FileExists(to.c_str()) == true) {

		FileInfo fileInfoFrom(derivativePath);
		FileInfo fileInfoTo(to);
		if (fileInfoFrom.getCrc() != fileInfoTo.getCrc()) {
			if (force != true) {
				logger.log(LOG_OK, CLogger::Level::INFO, "Changes may be lost by checkout, file not copied\"%s\"?", targetRelPath);
				ErrorCode::setErrorCode(SIA_ERROR::CHANGE_MAY_BE_LOST);
				return false;
			}
			else {
				logger.log(LOG_OK, CLogger::Level::INFO, "Forcing copy. This will overwrite changes, \"%s\"?", targetRelPath);
			}
		}
		else {
			if (force != true) {
				logger.log(LOG_OK, CLogger::Level::INFO, "File not copied as the images are the same, \"%s\"?", targetRelPath);
				ErrorCode::setErrorCode(SIA_ERROR::NO_CHANGE_IN_IMAGE);
				return false;
			}
			else {
				logger.log(LOG_OK, CLogger::Level::INFO, "Forcing copy. However the images are the same, \"%s\"?", targetRelPath);
				return true;
			}
		}
	}


	if (SAUtils::copy(derivativePath.c_str(), to.c_str()) == false) {
		logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to copy version to destination: \"%s\"?", to.c_str());
		return false;
	}
	return true;
}

bool VersionControl::uncheckoutFile(const char *filepath, const char *comment, bool force) {
	CLogger &logger = CLogger::getLogger();
	//CDCheckInOutManager &cd = CDCheckInOutManager::get();
	CheckoutStatus checkoutStatus;
	// Is checked out, if out then can not be unchecked out
	if (checkoutStatus.isCheckedOut(filepath) == false) {
		if (ErrorCode::getErrorCode() == SIA_ERROR::ALREADY_CHECKED_IN) {
			// Needs chacking for changes beween the one in the workspace and the one checked in
			// even if is already chected in, the user needs to if this is the case.
			return false;
		}
	}
	// Check for changes between the checked out images and the last version

	if (checkoutStatus.uncheckoutUpdate(filepath, comment) == false) {
		logger.log(LOG_UNABLE_TO_UNCHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to uncheckout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
		return false;
	}
	/*
	try {

	if (cd.CheckIn(CDCheckInOutManager::DB::Workspace, filepath) == false) {
	logger.log(LOG_UNABLE_TO_UNCHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to uncheckout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
	return false;
	}

	}
	catch (SIAAppException &e) {
	printf("Error: %s\n", e.what());
	if (checkoutStatus.checkinUpdate(filepath, comment) == false) {
	logger.log(LOG_UNABLE_TO_UNCHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to uncheckout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
	return false;
	}
	return false;
	}
	*/
	return true;
}

bool VersionControl::getVersions(const char * filepath, const char * versions, bool force)
{
	CLogger &logger = CLogger::getLogger();
	
	VersionIndex versionIndex;
	
	int latestVersion = -1;
	if ((latestVersion = versionIndex.getLatest(filepath)) == -1) {
		return false;
	}
	VersionScope versionScope(latestVersion);
	versionScope.scope(versions);
	bool done = false;
	std::string workspaceVersionPath = m_workspace;
	for (int i = 0; i <= latestVersion; i++) {
		
		if (versionScope.isInScope(i) == true) {
			if (!done) {
				PathController workspacePathControl(m_workspace.c_str());
				workspacePathControl.splitShort(filepath);
				
				workspaceVersionPath += '/';
				workspaceVersionPath += workspacePathControl.getYear();
				workspaceVersionPath += '/';
				workspaceVersionPath += workspacePathControl.getYearday();
				workspaceVersionPath += '/';
				workspaceVersionPath += SAUtils::getFilenameNoExt(workspacePathControl.getImageName());
				if (SAUtils::FileExists(workspaceVersionPath.c_str()) == false) {
					if (SAUtils::mkDir(workspaceVersionPath.c_str()) == false) {
						logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to create version folder: \"%s\"?", workspaceVersionPath.c_str());
						return false;
					}
				}

				

				done = true;
			}

			if (versionIndex.setRowCursor(filepath, i) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to find version: \"%d\"?", i);
				return false;
			}

			SharedMTRow row = versionIndex.getCurrentRow();
			MTColumn& col = row->columnAt(DB_VERSIONPATH);
			std::string shortImagePath = col.getString();
			col = row->columnAt(DB_FILENAME);
			std::string imageName = col.getString();
			int version = row->columnAt(DB_VERSION).getInt();
			unsigned int crc = row->columnAt(DB_CRC).getInt();
			std::string md5 = row->columnAt(DB_MD5).getString();
			
std::string to = workspaceVersionPath;
to += '/';
to += imageName;
std::string dbPath;
if (i == 0) {
	std::string root = m_pathToMaster.c_str();

	PathController masterPathControl(root.c_str());
	masterPathControl.splitShort(shortImagePath.c_str());
	if (masterPathControl.makeImagePath()) {
		return false;
	}
	dbPath = root;
	dbPath += '/';
	dbPath += masterPathControl.getYear();
	dbPath += '/';
	dbPath += masterPathControl.getYearday();
	dbPath += "/images/";
	dbPath += masterPathControl.getImage();
}
else {
	std::string root = m_pathToDerivative.c_str();

	PathController derivativePathControl(root.c_str());
	derivativePathControl.splitShort(shortImagePath.c_str());
	if (derivativePathControl.makeImagePath()) {
		return false;
	}
	dbPath = root;
	dbPath += '/';
	dbPath += derivativePathControl.getYear();
	dbPath += '/';
	dbPath += derivativePathControl.getYearday();
	dbPath += "/images/";
	dbPath += derivativePathControl.getImage();

}

if (SAUtils::copy(dbPath.c_str(), to.c_str()) == false) {
	logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to copy version to destination: \"%s\"?", to.c_str());
	return false;
}

		}
	}
	return false;
}




bool VersionControl::makeVersionsList(const char * filepath)
{
	CLogger &logger = CLogger::getLogger();

	VersionsList versionsList;
	VersionIndex versionIndex;

	int latestVersion = -1;
	if ((latestVersion = versionIndex.getLatest(filepath)) == -1) {
		return false;
	}

	
	

	
	std::string workspaceVersionPath = m_workspace;

	PathController workspacePathControl(m_workspace.c_str());
	workspacePathControl.splitShort(filepath);

	workspaceVersionPath += '/';
	std::string versionPath = workspacePathControl.getYear();
	versionPath += '/';
	versionPath += workspacePathControl.getYearday();
	versionPath += "/.sia/metadata/";
	versionPath += workspacePathControl.getImageName();
	workspaceVersionPath += versionPath;
	
	for (int i = 0; i <= latestVersion; i++) {
		if (i == 0) {
			continue;
		}

		if (versionIndex.setRowCursor(filepath, i) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to find version: \"%d\"?", i);
			return false;
		}

		SharedMTRow row = versionIndex.getCurrentRow();
		MTColumn& col = row->columnAt(DB_FILENAME);
		std::string imageName = col.getString();

		std::string fileName = imageName + ".xml";
		std::string thisVersionPath = workspaceVersionPath;
		thisVersionPath += '/';
		thisVersionPath += fileName;
		if (SAUtils::FileExists(thisVersionPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to create version folder: \"%s\"?", workspaceVersionPath.c_str());
			return false;
		}

		versionsList.add(i, versionPath.c_str());

	}
	std::string listFilePath = workspaceVersionPath;
	listFilePath += "/list.xml";

	XMLWriter xmlWriter;
	if (xmlWriter.writeImageVersionList(versionsList, filepath, listFilePath.c_str()) == false) {
		return false;
	}
	return true;
}

} /* namespace simplearchive */

#ifdef THIS_CODE_IS_NOT_USED
// filepath is the relative path from the archive root
bool VersionControl::checkout(const char *filepath, const char *comment) {

	CLogger &logger = CLogger::getLogger();
	ImagePath imagePath(filepath);
	std::string path = imagePath.getDataPath();
	std::string fullpath = path + '/' + imagePath.getImageName();
	std::string hstpath = imagePath.getLocalMasterHistoryPath() + '/' + imagePath.getImageName() + ".hst";
	std::string lockpath = imagePath.getLocalMasterHistoryPath() + '/' + imagePath.getImageName();

	// Lock the lock file
// commented out for testing
	if ((ImageLock::lock(lockpath.c_str())) == false) {
		logger.log(LOG_OK, CLogger::Level::INFO, "Cannot lock Image: \"%s\"", imagePath.getImageName().c_str());
		return false;
	}

	
	// Add the comment
	Version version(fullpath.c_str());
	int idx = version.getVersion();
	version.CopyDataVersion2Old();
	setHistory(hstpath.c_str(), filepath, comment, HistoryEvent::CHECKOUT, idx);
//	ImageHistory &imageHistory = ImageHistory::getImageHistory();
//	imageHistory.add(filepath, idx, comment, HistoryEvent::CHECKOUT);
	return true;
}

bool VersionControl::CopyNewVersion2Data(const std::string &newImagePath,const std::string &dataPath,const std::string &imageName) {
	std::string from = newImagePath + '/' + imageName;
	std::string to = dataPath + "/" + imageName;
	// data
	
	// Old
	//if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
	//	return false;
	//}


	return true;
}

/*
bool VersionControl::CopyNewVersion2Data(const std::string &newImagePath, const std::string &dataPath, const std::string &imageName) {
	std::string from = newImagePath + '/' + imageName;
	std::string to = dataPath + "/" + imageName;
	// data

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	return true;
}
*/

bool VersionControl::checkin(const char *filepath, const char *comment) {
	ImagePath imagePath(filepath);
	
	std::string path = imagePath.getDataPath();
	std::string fullpath = path + '/' + imagePath.getImageName();
	//getPathAndFilename(fullPath.c_str(), filename, path);
	
	std::string hstpath = imagePath.getLocalMasterHistoryPath() + '/' + imagePath.getImageName() + ".hst";
	std::string lockpath = imagePath.getLocalMasterHistoryPath() + '/' + imagePath.getImageName();

	if ((ImageLock::unlock(lockpath.c_str())) == false) {
		//return false;
	}

	
	Version version(fullpath.c_str());
	if (version.HasChanged() == false) {
		// check the image has changed
		uncheckout(filepath, comment);
		printf("The image has not changed\"%s\"", filepath);
		//return true;
	}
	std::string archivePath = imagePath.getImagePath() + '/' + imagePath.getImageName();
	const char *newNamePath = version.newVersion();
	//CopyNewVersion2Data(imagePath.getYyyymmddStrPath(), imagePath.getDataPath(), imagePath.getImageName());
	std::string metadataPath = imagePath.getLocalMasterMetadataPath() + '/' + imagePath.getImageName() + ".xml";
	//version.newVersionMetadata(metadataPath.c_str());
	// turn into log message
	//printf("New Version %s", newNamePath);
	// use the above for a log message
	int idx = version.getVersion();
	setHistory(hstpath.c_str(), filepath, comment, HistoryEvent::CHECKIN, idx);
//	ImageHistory &imageHistory = ImageHistory::getImageHistory();
//	imageHistory.add(filepath, idx, comment, HistoryEvent::CHECKIN);

	//ArchiveObject& archiveObject = ArchiveObject::get();
	//archiveObject.checkin(filepath, comment);

	// use the above for a log message
	return true;
}

bool VersionControl::uncheckout(const char *filepath, const char *comment) {
	ImagePath imagePath(filepath);

	std::string path = imagePath.getDataPath();
	std::string fullpath = path + '/' + imagePath.getImageName();
	//getPathAndFilename(fullPath.c_str(), filename, path);

	std::string hstpath = imagePath.getLocalMasterHistoryPath() + '/' + imagePath.getImageName() + ".hst";
	std::string lockpath = imagePath.getLocalMasterHistoryPath() + '/' + imagePath.getImageName();

	if ((ImageLock::unlock(lockpath.c_str())) == false) {
		//return false;
	}
	Version version(fullpath.c_str());

	// use the above for a log message
	int idx = version.Revert();
	if (idx < 0) {
		// log error
	}
	setHistory(hstpath.c_str(), filepath, comment, HistoryEvent::UNCHECKOUT, idx);
//	ImageHistory &imageHistory = ImageHistory::getImageHistory();
//	imageHistory.add(filepath, idx, comment, HistoryEvent::UNCHECKOUT);

	// use the above for a log message
	return true;

}

int VersionControl::getNumberOfVersions(const char *imagePath) {
	std::string fullPath = m_pathToArchive + "/" + imagePath;
	Version version(fullPath.c_str());
	int revision = version.getVersion();
	revision++;
	return revision;
}

bool VersionControl::getImageVersion(const char *imagePath,const char *targetPath, int idx) {
	std::string fullPath = m_pathToArchive + "/" + imagePath;
	if (SAUtils::FileExists(fullPath.c_str()) == false) {
		std::string message = "Path to checkout image is invalid \"";
		message += imagePath;
		message += "\"";
		throw SIAAppException(message);
	}
	if (SAUtils::DirExists(targetPath) == false) {
		std::string message = "Path to checkout image is invalid \"";
		message += imagePath;
		message += "\"";
		throw SIAAppException(message);
	}
	Version version(fullPath.c_str());
	version.setVersion(idx);
	version.setToVersion(idx);
	return true;
}

bool VersionControl::uncheckout(const char *imagePath,const char *targetPath, int idx, const char *comment) {
	
	// use the above for a log message
	return true;

}


bool VersionControl::setHistory(const char *hstpath, const char *filepath, const char *comment, HistoryEvent evt, int idx) {
	//ImageHistory imageHistory(hstpath);
	std::string idxStr = versonString(idx);
	//imageHistory.add(filepath, idxStr.c_str(), comment, HistoryEvent::UNCHECKOUT);
	/*
	ImageHistory &history = ImageHistory::getImageHistory(filepath);
	HistoryEvent he(HistoryEvent::CHECKIN);

	history.add(filepath, idxStr.c_str(), comment, he);
	*/
	return true;
}

std::string VersionControl::versonString(int idx) {
	std::stringstream s;
	s << std::setw(4) << std::setfill('0') << std::hex << idx;
	return s.str();
}

bool VersionControl::getPathAndFilename(const char *filepath, std::string &filename, std::string &path) {
	std::string fullPath = filepath;

	if (SAUtils::FileExists(fullPath.c_str()) == false) {
		std::string message = "Path to checkout image is invalid \"";
		message += filepath;
		message += "\"";
		throw SIAAppException(message);
	}

	// This fines the filename from path NEEDS TO BE MOVED INTO UTILS
	int slashpos = fullPath.find_last_of("/");
	if (slashpos != -1) {
		filename = fullPath.substr(slashpos+1, fullPath.length() - slashpos);
		path = fullPath.substr(0, slashpos);
	} else {
		std::string message = "Path to checkout image is invalid \"";
		message += filepath;
		message += "\"";
		throw SIAAppException(message);
	}
	return true;
}

/*
std::shared_ptr<ImageHistoryLog>  VersionControl::getHistory(const char *filepath) {
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;

	getPathAndFilename(fullPath.c_str(), filename, path);
	std::string hstpath = path + "/.metadata/" + filename + ".hst";

	ImageHistory imageHistory(hstpath.c_str());
	return imageHistory.get();
}
*/

} /* namespace simplearchive */
#endif
