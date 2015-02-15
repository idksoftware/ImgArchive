/*
 * ArchiveBuilder.h
 *
 *  Created on: May 7, 2014
 *      Author: wzw7yn
 */

#ifndef ARCHIVEBUILDER_H_
#define ARCHIVEBUILDER_H_
#include <string>

namespace simplearchive {

class ImageIndex;
class ImageSets;
class MetadataObject;
class CImageId;
class ExifObject;
class ImagePath;
class CSVDBFile;
class ImageContainer;
class HistoryEvent;
class ArchiveDate;
class ViewManager;
/**
 * @brief This class is the archive builder class. It contains the top level
 * processes to prepare the images to archived.
 *
 * Import
 * checkout 
 * checkin
 */
class ArchiveBuilder {
	bool m_Error;
	
	std::string m_RootOutputDir;
	std::string m_indexPath;
	std::string m_userRootdir;
	std::string m_metatemplatePath;
	ImageIndex *m_imageIndex;
	ArchiveDate *m_archiveDate;
	ViewManager *m_viewManager;
	bool m_useExternalExifTool;
	void copyExif(MetadataObject* metadataObject, ExifObject *exifObject);
	bool CreateMetadataXMLFile(ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject);
	bool CreateImage(CImageId &imageId, ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject);
	bool processHistory(const char *filepath, const char *comment, const HistoryEvent &he, int ver);
public:
	ArchiveBuilder();
	/// @brief This initalises the archive builder
	
	/// @param appdir - Index Path
	/// @param userrootdir - secondary root archive folder
	bool Init();
	/// @param sourcePath - source folder were the images to be imported reside.
	bool Import(const char *sourcePath);
	/// @brief Checkout
	/// @param fileath - distination folder were the images to be checked out reside.
	bool checkout(const char *filepath, const char *comment);
	/// @brief Checkin
	/// @param fileath - distination folder were the images to be checked out reside.
	bool checkin(const char *filepath, const char *comment);
	/// Distructor
	virtual ~ArchiveBuilder();
	/// @brief The 
	bool IsError()
	{
		return (!m_Error);
	}
	/// Sets the SIA to use external EXIF tool
	void setUseExternalExifTool(bool v) {
		m_useExternalExifTool = v;
	}
};

} /* namespace simplearchive */
#endif /* ARCHIVEBUILDER_H_ */
