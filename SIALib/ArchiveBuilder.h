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

#ifndef ARCHIVEBUILDER_H_
#define ARCHIVEBUILDER_H_
#include <string>

namespace simplearchive {

class ImageIndex;
class ImageSets;
class MetadataObject;
class BasicExif;
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
	
	std::string m_shadowPath;
	std::string m_indexPath;
	std::string m_workspacePath;
	std::string m_metatemplatePath;
	ImageIndex *m_imageIndex;
	ArchiveDate *m_archiveDate;
	
	bool m_useExternalExifTool;
	void copyBasicExit(MetadataObject& metadataObject, const BasicExif &eb);
	void copyExternalExif(MetadataObject& metadataObject, ExifObject &exifObject);
	//bool CreateMetadataXMLFile(ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject);
	bool CreateImage(const BasicExif &basicExif, ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject);
	
	bool processHistory(ImagePath &imagePath, const char *filepath, const char *comment, const HistoryEvent &he, int ver);
	void print(ExifObject &eo);
	void print(const BasicExif &be);
	void print(const MetadataObject& mo);
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
	bool ArchiveBuilder::uncheckout(const char *filepath, const char *comment);
	bool uncheckout(const char *filepath);
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
