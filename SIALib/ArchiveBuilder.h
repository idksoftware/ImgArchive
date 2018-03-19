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
#include <memory>
#include <string>

namespace simplearchive {

class ImageIndex;
class ImageSets;
class MetadataObject;
class BasicMetadata;
class ExifObject;
class ImportJournal;
/**
 * @brief This class is the archive builder class. It contains the top level
 * processes to prepare the images to archived.
 *
 * Import
 * checkout 
 * checkin
 */
class ArchiveObject;
class ArchiveBuilder {
	bool m_Error;
	
	std::string m_MasterPath;
	std::string m_indexPath;
	std::string m_workspacePath;
	std::string m_templatePath;

	int m_folders;
	int m_imageFiles;
	int m_imageFilesRejected;
	int m_imageFilesCompleted;

	ArchiveObject &m_archiveObject;
	bool m_useExternalExifTool;
	bool m_doDryRun;
	void copyBasicExit(MetadataObject& metadataObject, const BasicMetadata &eb);
	void copyExternalExif(MetadataObject& metadataObject, ExifObject &exifObject);
	
	void print(ExifObject &eo);
	void print(const BasicMetadata &be);
	void print(const MetadataObject& mo);
	std::shared_ptr<ImageSets> processFiles(const char *sourcePath, ImportJournal& importJournal);
	bool processImageGroupSets(std::shared_ptr<ImageSets> imageSets, ImportJournal& importJournal);
public:
	ArchiveBuilder(ArchiveObject &archiveObject);
	/// @brief This initalises the archive builder
	
	/// @param appdir - Index Path
	/// @param userrootdir - secondary root archive folder
	bool Init();
	/// @param sourcePath - source folder were the images to be imported reside.
	bool Import(const char *sourcePath, bool peekImport = false);
	bool ImportFile(const char *filePath);
	
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
