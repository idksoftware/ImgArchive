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
	static std::string m_versionPath;
	const char *getVersionPath() {
		return m_versionPath.c_str();
	}
	std::shared_ptr<MTRow> m_currow;
	bool isValid;
	int m_current;
public:
	VersionIndex();
	~VersionIndex();
	static void setVersionPath(const char *s) {
		m_versionPath = s;
	}
	bool createMasterVersion(const BasicMetadata &bm, const char *path, size_t  masterSeqNumber, size_t  primarySeqNumber);
	bool createDerivativeVersion(const VersionMetadataObject& vmo, const char *path, int derivativeSeqNumber, int primarySeqNumber, const char *masterFileName);
	std::string getCurrentVersion(const char *path);
	SharedMTRow getCurrentRow();
	bool setRowCursor(const char *path);
	bool setRowCursor(const char *path, int version);
	int getLatest(const char *path) {
		if (m_current == -1) {
			if (setRowCursor(path) == false) {
				return -1;
			}
		}
		return m_current;
	}

	
};

}