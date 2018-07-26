#include "VersionIndex.h"
#include "VersionMetadataObject.h"
#include "BasicMetadata.h"
#include "SAUtils.h"
#include "pathcontroller.h"
#include "FileInfo.h"

namespace simplearchive {

	
VersionIndex::VersionIndex() : versionTable(new VersionTable)
{
	isValid = false;
}


VersionIndex::~VersionIndex()
{
}

bool VersionIndex::createMasterVersion(const BasicMetadata &bm, const char *path, int masterSeqNumber, int primarySeqNumber) {
	bm.columnAt(DB_FILEPATH).toString() = path;
	int crc = bm.columnAt(DB_CRC).getInt();
	std::string filenameStr = bm.columnAt(DB_FILENAME).toString();
	filenameStr += ".csv";
	PathController pathController;
	pathController.splitShort(path);
	std::string rel = path;
	std::string rootPath = getVersionPath();
	if (!PathController::validateFullYYMMDD(rootPath.c_str(), path)) {
		return false;
	}
	if (!pathController.createPath(rootPath.c_str(), path)) {
		return false;
	}
	
	//if (PathController::validateFullYYMMDD(rootPath.c_str(), pathController.getShortRelativePath().c_str())) {
	//	return false;
	//}
	//if (PathController::createPath(rootPath.c_str(), pathController.getShortRelativePath().c_str())) {
	//	return false;
	//}
	
	rel += "/";
	rel += filenameStr;
	SharedMTRow row;
	if (SAUtils::FileExists(rel.c_str()) == false) {
		row = versionTable->makeRow();
		row->join(bm);
	}
	else {
		versionTable->read(pathController.getFullPath().c_str(), filenameStr.c_str());
		row = versionTable->makeRow();
		row->join(bm);
	}
	row->columnAt(DB_DATABASEID) = masterSeqNumber;
	row->columnAt(DB_SEQUENCEID) = primarySeqNumber;
	
	const MTRow *r = row.get();
	versionTable->addRow(*r);
	versionTable->write(pathController.getFullPath().c_str(), filenameStr.c_str());
	versionTable->truncate();
	isValid = true;
	return true;
}

bool VersionIndex::createDerivativeVersion(const VersionMetadataObject& vmo, const char *path, int derivativeSeqNumber, int primarySeqNumber) {

	PathController pathController;
	pathController.splitShort(path);
	std::string rel = path;
	std::string rootPath = getVersionPath();
	if (!PathController::validateFullYYMMDD(rootPath.c_str(), path)) {
		return false;
	}
	if (!pathController.createPath(rootPath.c_str(), path)) {
		return false;
	}
	

	
	std::string filenameStr = vmo.columnAt(DB_FILENAME).toString();
	filenameStr += ".csv";
	rel += "/";
	rel += filenameStr;

	versionTable->read(pathController.getFullPath().c_str(), filenameStr.c_str());

	vmo.columnAt(DB_DATABASEID) = derivativeSeqNumber;
	vmo.columnAt(DB_SEQUENCEID) = primarySeqNumber;

	versionTable->addRow(vmo);
	versionTable->write(pathController.getFullPath().c_str(), filenameStr.c_str());
	versionTable->truncate();
	isValid = true;
	return true;
}

bool VersionIndex::setRowCursor(const char *path) {
	std::string fullPath;
	isValid = false;
	PathController pathController;
	std::string filepath = path;
	filepath += ".csv";
	pathController.splitShort(filepath.c_str());

	std::string rootPath = getVersionPath();
	if (PathController::validateFullYYMMDD(rootPath.c_str(), path) == false) {
		ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
		return false;
	}
	pathController.setRoot(rootPath);
	std::string itemPath = rootPath;
	//itemPath += '/';
	//itemPath += path;
	pathController.makeImagePath();
	std::string verPath = pathController.getFullPath();
	pathController.splitPathAndFile(verPath.c_str());
	std::string filename = pathController.getImageName();

	if (versionTable->read(pathController.getRoot().c_str(), filename.c_str()) == false) {
		ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
		return false;
	}
	if (versionTable->empty()) {
		ErrorCode::setErrorCode(SIA_ERROR::IMAGE_NOT_FOUND);
		return false;
	}

	for (auto ii = versionTable->begin(); ii != versionTable->end(); ++ii) {
		currow = *ii;
	}
	isValid = true;
	return true;
}

std::string VersionIndex::getCurrentVersion(const char *path) {

	std::string fullPath;
	if (!setRowCursor(path) == false) {
		return fullPath;
	}
	fullPath = currow->columnAt(DB_FILEPATH).getString();
	fullPath += '/';
	fullPath += currow->columnAt(DB_FILENAME).getString();
	return fullPath;
}

SharedMTRow VersionIndex::getCurrentRow() {
	
	SharedMTRow row = versionTable->makeRow();
	row->join(*currow);
	return row;
}


}
