/*
imageContainer * MetadataObject.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: wzw7yn
 */
#include "DBDefines.h"
#include "MetadataObject.h"
#include "BasicMetadata.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

MetadataSchema MetadataObject::m_tableSchema;
MetadataObject::MetadataObject() : MTRow(m_tableSchema) {}


MetadataObject::~MetadataObject() {
	//printf("+++ MetadataObject Deleting +++\n");
}

void MetadataObject::update(const FileInfo& fileInfo, int seqNo,  int version) {
	columnAt(static_cast<int>(MetadataObjectIndex::MD_SEQUENCEID_IDX)) = seqNo;
	columnAt(static_cast<int>(MetadataObjectIndex::MD_FILENAME_IDX)) = fileInfo.getName().c_str();
	columnAt(static_cast<int>(MetadataObjectIndex::MD_FILEPATH_IDX)) = fileInfo.getPath().c_str();
	columnAt(static_cast<int>(MetadataObjectIndex::MD_CRC_IDX)) = fileInfo.getCrc();
	columnAt(static_cast<int>(MetadataObjectIndex::MD_MD5_IDX)) = fileInfo.getMd5().c_str();
	columnAt(static_cast<int>(MetadataObjectIndex::MD_UUID_IDX)) = fileInfo.getUuid().c_str();
	columnAt(static_cast<int>(MetadataObjectIndex::MD_FILESIZE_IDX)) = fileInfo.getSize();
	columnAt(static_cast<int>(MetadataObjectIndex::MD_DATEMODIFIED_IDX)) = fileInfo.getModTime();
	columnAt(static_cast<int>(MetadataObjectIndex::MD_DATECREATE_IDX)) = fileInfo.getCreateTime();
	columnAt(static_cast<int>(MetadataObjectIndex::MD_VERSION_IDX)) = version;
}

} /* namespace simplearchive */
