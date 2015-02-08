/*
 * CSVDatabase.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: wzw7yn
 */


#include "MediaProperties.h"
#include "CSVDatabase.h"
#include "AssetProperties.h"
#include "CameraInformation.h"

#include "GPSProperties.h"

#include "CopyrightProperties.h"

namespace simplearchive {

class FolderSet {
	AssetProperties m_assetProperties;
	CameraInformation m_cameraInformation;
	GPSProperties m_GPSProperties;
	MediaProperties m_MediaProperties;
	CopyrightProperties m_copyrightProperties;
	std::string m_path;
public:
	FolderSet(const char *path) {
		m_path = path;
	}
	~FolderSet() {

	}
	void add(MetadataObject &metadataObject);
};


void FolderSet::add(MetadataObject &metadataObject) {

	m_assetProperties.read(m_path.c_str());
	m_assetProperties.add(metadataObject);
	m_assetProperties.write(m_path.c_str());
	m_cameraInformation.read(m_path.c_str());
	m_cameraInformation.add(metadataObject);
	m_cameraInformation.write(m_path.c_str());
	m_GPSProperties.read(m_path.c_str());
	m_GPSProperties.add(metadataObject);
	m_GPSProperties.write(m_path.c_str());
	m_MediaProperties.read(m_path.c_str());
	m_MediaProperties.add(metadataObject);
	m_MediaProperties.write(m_path.c_str());
	m_copyrightProperties.read(m_path.c_str());
	m_copyrightProperties.add(metadataObject);
	m_copyrightProperties.write(m_path.c_str());
}

CSVDatabase *CSVDatabase::m_this = 0;

CSVDatabase::CSVDatabase() {
}

CSVDatabase::~CSVDatabase() {
}

CSVDatabase &CSVDatabase::get() {
	if (m_this == 0) {
		m_this = new CSVDatabase;
	}
	return *m_this;
}

void CSVDatabase::add(MetadataObject &metadataObject, const char *path) {
	// check path exists
	FolderSet folderSet(path);
	folderSet.add(metadataObject);
}

} /* namespace simplearchive */
