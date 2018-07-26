#pragma once

#include <string>
#include <memory>
#include "ImageIndex.h"
#include "MetaType.h"

namespace simplearchive {

class VersionTable;
class MTRow;
class VersionMetadataObject;


class VersionIndex 
{
	std::shared_ptr<VersionTable> versionTable;
	std::string versionPath;
	const char *getVersionPath() {
		return versionPath.c_str();
	}
	std::shared_ptr<MTRow> currow;
	bool isValid;
public:
	VersionIndex();
	~VersionIndex();
	void setVersionPath(const char *s) {
		versionPath = s;
	}
	bool createMasterVersion(const BasicMetadata &bm, const char *path, int masterSeqNumber, int primarySeqNumber);
	bool createDerivativeVersion(const VersionMetadataObject& vmo, const char *path, int derivativeSeqNumber, int primarySeqNumber);
	std::string getCurrentVersion(const char *path);
	SharedMTRow getCurrentRow();
	bool setRowCursor(const char *path);
};

}