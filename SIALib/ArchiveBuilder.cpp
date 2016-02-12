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

#include "AppConfig.h"
#include "SACmdArgs.h"
#include "ArchiveDate.h"
#include "ArchiveBuilder.h"
#include "StandardOut.h"
#include "ArchiveRepository.h"
#include "ImageExtentions.h"
#include "ExifObject.h"
#include "MetadataTemplate.h"
#include "MetadataObject.h"
#include "ImageFileReader.h"
#include "ImageIndex.h"
#include "SAUtils.h"
#include "CLogger.h"
#include "ImageGroup.h"
#include "TargetsList.h"
#include "FileInfo.h"
#include "BasicExifFactory.h"
#include "XMLWriter.h"
#include "ImagePath.h"
#include "CSVDBFile.h"
#include "SIAApplicationState.h"
#include "CSVDatabase.h"
#include "HookCmd.h"
#include "ViewManager.h"
#include "VersionControl.h"
#include "ImportJournal.h"
#include "CheckoutStatus.h"
#include "ContentsLister.h"
#include "ExportImages.h"
#include "CheckFile.h"
#include "ArchiveObject.h"
#include <stdio.h>
#include <sstream>
#include <vector>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	class FilesImport {
		int m_folderCount;
		int m_fileCount;
		ImageSets *m_imageSets;
		ImageSet *m_imageSet;
		std::string m_path;
	public:
		FilesImport() {
			
		};
		bool add(const char *filePath) {
			CLogger &logger = CLogger::getLogger();
			if (m_imageSets == 0) {
				m_imageSets = new ImageSets;
			}
			ImageExtentions& ie = ImageExtentions::get();
			std::string file = SAUtils::getFilename(filePath);
			if (ie.IsValid(file.c_str()) == false) {
				logger.log(LOG_OK, CLogger::WARNING, "Not a valid file type \"%s\" rejecting ", file.c_str());
				return false;
			}
			std::string folder = SAUtils::getFolder(filePath);
			ImageSet *imageSet = m_imageSets->find(folder.c_str());
			if (imageSet == nullptr) {
				imageSet = new ImageSet(filePath);
				m_imageSets->insert(m_imageSets->end(), imageSet);
			}
			imageSet->insert(imageSet->end(), new ImageItem(filePath));
			return true;
		}
		ImageSets *getImageSets() { return m_imageSets; };
	};

	class ImportList : public std::vector<std::string> {
	public:
		ImportList() {};
		virtual ~ImportList() {};
	};

	class ImportListReader {
	public:
		std::unique_ptr<ImportList> Read(const char *filepath) {
			std::unique_ptr<ImportList> importFile(new ImportList);
			char text[MAX_PATH];
			std::ifstream file(filepath);
			if (file.is_open() == false) {
				// empty?
				return false;
			}

			while (file.getline(text, 100)) {
				importFile->push_back(text);
			}
			file.close();
			
			return importFile;
		}
	};

	class ImageProcessor {
		
		CSVDBFile &m_csvDBFile;
		CSVDatabase &m_csvDatabase;
		std::unique_ptr<ImagePath> m_imagePath;
		ImageContainer *m_curImageSet;
		std::unique_ptr<ArchiveDate> m_archiveDate;
		std::string& m_workspacePath;
		ImageIndex& m_imageIndex;
	public:
		
		ImageProcessor(ImageContainer *curImageSet, CSVDBFile &csvDBFile, CSVDatabase &csvDatabase, std::string& workspacePath, ImageIndex& imageIndex)
			: m_csvDBFile(csvDBFile), m_csvDatabase(csvDatabase), m_curImageSet(curImageSet), m_workspacePath(workspacePath), m_imageIndex(imageIndex)
		{
			CLogger &logger = CLogger::getLogger();

			
			if (curImageSet->getError() < 0) {
				logger.log(LOG_OK, CLogger::ERR, "Error processing image not archiving: \"%s\"", curImageSet->getName().c_str());
				return;
			}
			curImageSet->PostProcess();

			m_archiveDate.reset(new ArchiveDate);
			if (m_archiveDate->process(*curImageSet) == false) {
				logger.log(LOG_OK, CLogger::ERR, "Error processing image archive date, not archiving: \"%s\"", curImageSet->getName().c_str());
				return;
			}

			ExifDate archiveDate = m_archiveDate->getArchiveDate();
			curImageSet->setTime(archiveDate.getTime());
			m_imagePath.reset(new ImagePath(archiveDate.getTime()));
			
			if (curImageSet->getError() < 0) {
				logger.log(LOG_OK, CLogger::ERR, "Error processing image not archiving: \"%s\"", curImageSet->getName().c_str());
				return;
			}

			logger.log(LOG_OK, CLogger::INFO, "Image: %s set to archive date: \"%s\"", curImageSet->getName().c_str(), archiveDate.toString().c_str());
			if (curImageSet->getComment().empty() == true) {
				curImageSet->setComment("New Image");
			}

			std::string imageRootPath = curImageSet->getPath();
			logger.log(LOG_OK, CLogger::INFO, "Image(s) source path: \"%s\"", imageRootPath.c_str());
			curImageSet->setImageRootPath(m_imagePath->getRelativePath());
			if (curImageSet->hasPictureFile()) {
				const BasicExif &basicExif = curImageSet->getPictureId();
				const MetadataObject &metadataObject = *curImageSet->getPictureMetadata();
				std::string picName = curImageSet->getPictureFile();
				std::string path = curImageSet->getPath();
				if (processImage(picName, path, metadataObject, basicExif) == false) {
					return;
				}
				
			}
			if (curImageSet->hasRawFile()) {
				const BasicExif &basicExif = curImageSet->getRawId();
				const MetadataObject &metadataObject = *curImageSet->getRawMetadata();
				std::string picName = curImageSet->getRawFile();
				std::string path = curImageSet->getPath();
				if (processImage(picName, path, metadataObject, basicExif) == false) {
					return;
				}
				
			}
			
		}
		bool processImage(std::string& picName, std::string& path, const MetadataObject &metadataObject, const BasicExif &basicExif) {
			MetadataObject &temp = (MetadataObject &)metadataObject;
			CLogger &logger = CLogger::getLogger();

			m_imagePath->setImageName(picName.c_str());
			int seqNumber = m_csvDBFile.getNextIndex();
			temp.setSequenceId(seqNumber);
			ExifDateTime timeNow;
			timeNow.now();
			temp.setDateAdded(timeNow);
			m_imagePath->setCurrentSourcePath(m_curImageSet->getPath().c_str());
			// Create metadata xml file for this image.
			if (CreateImage(basicExif, *m_imagePath, m_csvDBFile, temp) == false) {
				return false;
			}
			std::string shortFilePath = m_imagePath->getYyyymmddStr() + "/" + m_imagePath->getImageName();
			/*
			std::string filepath = m_imagePath->getRelativePath() + '/' + m_imagePath->getImageName();
			
			// main
			if (m_imagePath->copyFile(m_curImageSet->getPath(), picName) == false) {
				return false;
			}
			
			

			//std::string shortPath = 
			m_csvDatabase.add(temp, shortFilePath.c_str());
			
			if (ViewManager::GetInstance().add(seqNumber) == false) {
				return false;
			}
			*/
			ImportJournal& importJournal = ImportJournalManager::GetJournal();
			std::string from = path + "/" + picName;
			importJournal.update(from.c_str(), ImportJournal::Imported, shortFilePath.c_str());

			PostArchiveCmd postArchiveCmd(*m_imagePath);
			postArchiveCmd.process();
			logger.log(LOG_OK, CLogger::SUMMARY, "Archiving image: \"%s\"", picName.c_str());

			m_imageIndex.updatePath(basicExif.getCrc(), shortFilePath.c_str());

			processHistory(*m_imagePath, shortFilePath.c_str(), m_curImageSet->getComment().c_str());
			return true;
		}

		bool CreateImage(const BasicExif &basicExif, ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject) {
			
			return ArchiveObject::get().CreateImage(basicExif, imagePath, csvDBFile, metadataObject);
		}
		
		
		bool processHistory(ImagePath &imagePath, const char *filepath, const char *comment) {
			
			return ArchiveObject::get().processHistory(imagePath, filepath, comment);
		}
		
		ImagePath& getImagePath() {
			return *m_imagePath;
		}
	};

	ArchiveBuilder::ArchiveBuilder(ArchiveObject &archiveObject) : m_archiveObject(archiveObject) {
		m_Error = false;
		
		m_useExternalExifTool = false;

	}

	ArchiveBuilder::~ArchiveBuilder() {
		
	}

	bool ArchiveBuilder::Init() {
		CAppConfig config = CAppConfig::get();
		m_Error = false;
		m_shadowPath = config.getShadowPath();
		m_indexPath = config.getIndexPath();
		m_workspacePath = config.getWorkspacePath();
		m_metatemplatePath = config.getMetadataTemplatePath();

		std::string shadowJournalPath = ImagePath::getShadowJournalPath();
		ImportJournalManager::setJournalFilePath(shadowJournalPath.c_str());

		ViewManager::GetInstance().initaliseMaster(config.getWorkspacePath(), config.getMasterViewPath());

		CLogger &logger = CLogger::getLogger();

		logger.log(LOG_OK, CLogger::SUMMARY, "Using Shadow folder: \"%s\"", m_shadowPath.c_str());
		if (SAUtils::DirExists(m_shadowPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::FATAL, "Archive Repository directory not accessable? \"%s\"", m_shadowPath.c_str());
			m_Error = true;
		}
		logger.log(LOG_OK, CLogger::INFO, "Found Archive Repository directory \"%s\"", m_shadowPath.c_str());


		if (SAUtils::DirExists(m_indexPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::FATAL, "Archive Index directory not accessable? \"%s\"", m_indexPath.c_str());
			m_Error = true;
		}
		logger.log(LOG_OK, CLogger::INFO, "Found Archive Index directory \"%s\"", m_indexPath.c_str());

		logger.log(LOG_OK, CLogger::SUMMARY, "Using Workspace folder: \"%s\"", m_shadowPath.c_str());
		if (SAUtils::DirExists(m_workspacePath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::FATAL, "Workspace directory not accessable? \"%s\"", m_workspacePath.c_str());
			m_Error = true;
		}
		logger.log(LOG_OK, CLogger::INFO, "Found Workspace Archive Repository directory \"%s\"", m_workspacePath.c_str());
		// Primary Index path
		m_archiveObject.getImageIndex().init(m_indexPath.c_str());
		
	
		std::string shadowPath = ImagePath::getShadowHistoryPath();
		
		CheckFile::initalise(config.getWorkspacePath());
		return (!m_Error);
	}


	bool ArchiveBuilder::ImportFile(const char *sourcePath) {
		ImportListReader importListReader;
		
		std::unique_ptr<ImportList> list = importListReader.Read(sourcePath);
		
		for (ImportList::iterator i = list->begin(); i != list->end(); i++) {
			std::string fileItem = *i;
			if (SAUtils::IsFile(fileItem.c_str())) {
				continue;
			}
			else {
				if (Import(fileItem.c_str()) == false) {
					return false;
				}
			}
		}
		return true;
	}

	bool ArchiveBuilder::Import(const char *sourcePath, bool peekImport) {
		CLogger &logger = CLogger::getLogger();
		if (SAUtils::DirExists(sourcePath) == false) {
			logger.log(LOG_OK, CLogger::ERR, "Source path \"%s\"not Found", sourcePath);
			return false;
		}
		// imageSets are no longer required so can be deleted.
		ImportJournal& importJournal = ImportJournalManager::GetJournal();
		ImageSets *imageSets = nullptr;
		if ((imageSets = processFiles(sourcePath, importJournal)) == nullptr) {
			return false;
		}
		if (peekImport == true) {
			return true;
		}
		if (processImageGroupSets(imageSets, importJournal) == false) {
			return false;
		}
		if (imageSets != nullptr) {
			delete imageSets;
		}
		//targetsList.destroy();
		return true;
	}

	ImageSets *ArchiveBuilder::processFiles(const char *sourcePath, ImportJournal& importJournal) {

		CLogger &logger = CLogger::getLogger();

		//
		// ==== Step 1 ====
		// Read files into folder sets (ImageSets)
		//
		logger.log(LOG_ANALISING, CLogger::INFO, "Stage 1: Placing Image files into folder sets");
		
		TargetsList targetsList;
		targetsList.process(sourcePath);

		logger.log(LOG_IMAGE_SUMMARY, CLogger::SUMMARY, "%d files to be processed", TargetsList::getFileCount());
		logger.log(LOG_FOLDER_SUMMARY, CLogger::SUMMARY, "%d Folders to be processed", TargetsList::getFolderCount());
		ImageSets *imageSets = nullptr;
		if ((imageSets = targetsList.getImageSets()) == nullptr) {
			// No images to process
			return nullptr;
		}
		return imageSets;
	}


	bool ArchiveBuilder::processImageGroupSets(ImageSets *imageSets, ImportJournal& importJournal) {

		CLogger &logger = CLogger::getLogger();
		SACmdArgs &saCmdArgs = SACmdArgs::get();
		CSVDatabase &csvDB = CSVDatabase::get();
		const char *seqPath = ImagePath::getShadowSequenceNumberPath().c_str();
		CSVDBFile csvDBFile(seqPath);

		std::string metatemplatePath = m_metatemplatePath;
		metatemplatePath += "/default.tpl";

		if (MetadataTemplate::read(metatemplatePath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::INFO, "Cannot read default template file \"%s\"", metatemplatePath.c_str());
		}
		logger.log(LOG_OK, CLogger::INFO, "Read metadata template file \"%s\"", metatemplatePath.c_str());
		// Creating metadata object with bulk data

		imageSets->processImportJournal(importJournal);
		imageSets->processHook();
		logger.log(LOG_OK, CLogger::INFO, "Found %d images to be processed", importJournal.numberOfImages());
		ImportJournalManager::save();
		// process folder sets into image groups
		//ImageGroups imageGroups;


		if (imageSets == nullptr) {
			return nullptr;
		}
		// ==== Step 2 ====
		// Read files into Image group sets (ImageGroups)
		//
		ArchiveRepository &archiveRepository = ArchiveRepository::get();
		archiveRepository.setPathToArchive(m_shadowPath);
		logger.log(LOG_IMPORTING, CLogger::ERR, "Stage 2: Processing Image files");
		archiveRepository.setPathToActiveRoot(this->m_workspacePath);
		/// Iterate the Image Sets
		
		for (std::vector<ImageSet *>::iterator i = imageSets->begin(); i != imageSets->end(); i++) {
			ImageSet *imageSet = *i;

			/// Create a Image Group container
			ImageGroup *imageGroup = new ImageGroup(imageSet->getPath());
			/// Insert into Image Group Sets
			//imageGroups.insert(imageGroups.end(), imageGroup);
			logger.log(LOG_OK, CLogger::SUMMARY, "Processing Image files in the source location %s", imageSet->getPath());
			/// Iterate the current image set
			for (std::vector<ImageItem *>::iterator i = imageSet->begin(); i != imageSet->end(); i++) {
				/// Image Item
				SIAApplicationState::RunState state = SIAApplicationState::getState();
				ImageItem *imageItem = *i;
				importJournal.setCurrent(imageItem->getPath());
				logger.log(LOG_CURRENT_IMAGE, CLogger::SUMMARY, "%d image\n", importJournal.getCurrentImageIndex());
				logger.log(LOG_IMAGE_NAME, CLogger::SUMMARY, "%s", imageItem->getFilename().c_str());
				logger.log(LOG_OK, CLogger::SUMMARY, "Processing Image file: %s", imageItem->getFilename().c_str());
				ExifObject *exifObject = nullptr;
				//data->print();	
				bool status = false;
				std::string path = imageItem->getPath();
				if (SAUtils::IsFile(imageItem->getPath()) == false) {
					continue;
				}
				ImageExtentions& ie = ImageExtentions::get();
				if (ie.IsValid(imageItem->getFilename().c_str()) == false) {
					logger.log(LOG_INVALID_FILE_TYPE, CLogger::WARNING, "Not a valid file type \"%s\" rejecting ", imageItem->getFilename().c_str());
					continue;
				}
				//logger.log(LOG_OK, CLogger::SUMMARY, ": %s", imageItem->getFilename().c_str());

				std::string pathstr = imageItem->getPath();
				logger.log(LOG_OK, CLogger::INFO, "Path of image: %s", pathstr.c_str());
				// This needs to be replaced with a factory class
				// ImageId object reads the image byte by byte for md5 and crc and Basic Exif.


				BasicExif_Ptr basicExifPtr = BasicExifFactory::make(pathstr);
				const BasicExif& basicExif = *basicExifPtr;
				print(basicExif);
				// Add Image to the Index database if not a dup.
				logger.log(LOG_OK, CLogger::INFO, "Find if dup %s", imageItem->getFilename().c_str());
				// If Make no changes == true, do not add this image into the dups index
				if (!saCmdArgs.IsMakeNoChanges()) { // DEBUG only allow dups
					// Force changes i.e the archive is damaged and images are being
					// added from a backup
					if (saCmdArgs.IsForceChanges()) {
						if (m_archiveObject.getImageIndex().IsDup(basicExif.getCrc())) {
							//m_imageIndex->getData(basicExif.getCrc());
							logger.log(LOG_OK, CLogger::INFO, "Dup %s", imageItem->getFilename().c_str());
							// reject image from import
							continue;
						}
					}
					else {
						// normal operation. dups are rejected
						int pos = -1;
						if (pos = m_archiveObject.getImageIndex().IsDup(basicExif.getCrc())) {
							//m_imageIndex->getData(imageId->getCrc());
							logger.log(LOG_DUPLICATE, CLogger::WARNING, "    Image \"%s\" was found to be a duplicate. Rejecting from import", imageItem->getFilename().c_str());
							// reject image from import
							ImageId imageId = m_archiveObject.getImageIndex().findDup(basicExif.getCrc());
							if (imageId.getName().empty()) {
								logger.log(LOG_OK, CLogger::ERR, "Image indexing corrupt %s", imageItem->getFilename().c_str());
							}
							else {
								importJournal.update(imageItem->getPath(), ImportJournal::Duplicate, imageId.getLocation().c_str());
								if (ImportJournalManager::save() == false) {
									logger.log(LOG_OK, CLogger::FATAL, "Unable to save Journal File");
									return false;
								}
							}
							continue;
						}
						else {
							// Add To the Image Indexing (used to find duplicates)
							if (m_archiveObject.getImageIndex().add(basicExif) == false) {
								logger.log(LOG_DUPLICATE, CLogger::WARNING, "    Image \"%s\" was found to be a duplicate. Rejecting from import", imageItem->getFilename().c_str());
								// reject image from import
								ImageId imageId = m_archiveObject.getImageIndex().findDup(basicExif.getCrc());
								if (imageId.getName().empty()) {
									logger.log(LOG_OK, CLogger::ERR, "Image indexing corrupt %s", imageItem->getFilename().c_str());
								}
								else {
									importJournal.update(imageItem->getPath(), ImportJournal::Duplicate, imageId.getLocation().c_str());
									if (ImportJournalManager::save() == false) {
										logger.log(LOG_OK, CLogger::FATAL, "Unable to save Journal File");
										return false;
									}
								}
								continue;
							}
							logger.log(LOG_OK, CLogger::INFO, "Adding image to dups index %s", imageItem->getFilename().c_str());
						}
					}
				}
				// Not a dup so add to group. 
				if (!basicExif.isExifFound()) {
					logger.log(LOG_OK, CLogger::INFO, "No EXIF simple EXIF infomation found in \"%s\"", imageItem->getFilename().c_str());
				}
				logger.log(LOG_OK, CLogger::INFO, "EXIF simple EXIF infomation found in \"%s\"", imageItem->getFilename().c_str());
				if (!basicExif.isExifFound() || m_useExternalExifTool) {
					// Try external Exif Tool
					logger.log(LOG_OK, CLogger::INFO, "Using external EXIF tool on file \"%s\"", imageItem->getFilename().c_str());
					exifObject = ImageFileReader::externalExifTool(pathstr);
				}
				//exifObject->debugPrint();
				if (exifObject == nullptr) {
					logger.log(LOG_OK, CLogger::INFO, "External EXIF reader not installed. No extra EXIF read for image\"%s\"", imageItem->getFilename().c_str());
				}
				else {
					logger.log(LOG_OK, CLogger::INFO, "External EXIF reader completed for image\"%s\"", imageItem->getFilename().c_str());
					print(*exifObject);
				}
				logger.log(LOG_OK, CLogger::INFO, "Processing bulk metedata %s", imageItem->getFilename().c_str());

				MetadataTemplate &metadataTemplate = MetadataTemplate::GetInstance();
				MetadataObject_ptr metadataObjectPtr = metadataTemplate.getMetadataObject();
				MetadataObject &metadataObject = *metadataObjectPtr;
				try {
					copyBasicExit(metadataObject, basicExif);
				}
				catch (std::out_of_range &e) {
					printf("%s\n", e.what());
				}
				if (exifObject != nullptr) {
					copyExternalExif(metadataObject, *exifObject);
				}
				print(metadataObject);
				imageGroup->add(basicExifPtr, metadataObjectPtr);
				logger.log(LOG_OK, CLogger::INFO, "completed step2 \"%s\"", imageItem->getFilename().c_str());
				/*
				XMLWriter xmlWriter;
				xmlWriter.writeImage(*metadataObject, "c:/temp/image.xml");
				*/
			}
			for (std::vector<ImageContainer *>::iterator i = imageGroup->begin(); i != imageGroup->end(); i++) {
				ImageContainer *imageSet = *i;
				ImageProcessor imageProcessor(imageSet, csvDBFile, csvDB, m_workspacePath, m_archiveObject.getImageIndex());
				
			}
		}

		ImportJournalManager::save();
		_CrtDumpMemoryLeaks();
		return true;
	}

/* AO
bool ArchiveBuilder::CreateImage(const BasicExif &basicExif, ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject) {
	unsigned long seqNumber = csvDBFile.getNextIndex();
	std::string seqNumberStr = std::to_string(seqNumber);
	metadataObject.setSequenceId(seqNumberStr);
	unsigned long n = basicExif.getSize();
	unsigned long crc = basicExif.getCrc();
	ExifDate date;
	date.now();

	// Write the metadata to the shadow archive
	std::string to = imagePath.getLocalShadowMetadataPath() + '/' + imagePath.getImageName() + ".xml";
	XMLWriter xmlWriter;
	if (xmlWriter.writeImage(metadataObject, to.c_str()) == false) {
		return false;
	}
	
	to = imagePath.getWorkspaceMetadataPath() + '/' + imagePath.getImageName() + ".xml";
	if (xmlWriter.writeImage(metadataObject, to.c_str()) == false) {
		return false;
	}
	

	if (csvDBFile.insert(seqNumber, imagePath.getRelativePath().c_str(), imagePath.getImageName().c_str(), n, crc, basicExif.getMd5().c_str(),
																									basicExif.getUuid().c_str(), 0, date) == false) {
		return false;
	}
	return true;
}
*/
// Create metadata xml file for this image.
/*
bool ArchiveBuilder::CreateMetadataXMLFile(ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject) {
	std::string to = imagePath.getLocalShadowMetadataPath() + '/' + imagePath.getImageName() + ".xml";
	XMLWriter xmlWriter;
	if (xmlWriter.writeImage(metadataObject, to.c_str()) == false) {
		return false;
	}
	unsigned long seqNumber = csvDBFile.getNextIndex();
	ExifDate date;
	date.now();
	FileInfo fileId(to);
	std::string relPath = imagePath.getRelativePath() + '/' + imagePath.getImageName() + ".xml";
	if (csvDBFile.insert(seqNumber, imagePath.getRelativePath().c_str(), imagePath.getImageName().c_str(), fileId.getSize(), fileId.getCrc(), fileId.getMd5().c_str(),
																			fileId.getUuid().c_str(), 0, date) == false) {
		return false;
	}
	return true;
}
*/

/* AO
bool ArchiveBuilder::processHistory(ImagePath &imagePath, const char *filepath, const char *comment, const HistoryEvent &he, int ver) {

	std::string fullPath = m_workspacePath + "/" + filepath;
	std::string filename;
	std::string path;

	// This fines the filename from path NEEDS TO BE MOVED INTO UTILS
	int slashpos = fullPath.find_last_of("/");
	if (slashpos != -1) {
		filename = fullPath.substr(slashpos + 1, fullPath.length() - slashpos);
		path = fullPath.substr(0, slashpos);
	}
	else {
		//printf("Path to Image in achive invalid \"%s\"", filepath);
	}
	std::string hstpath = imagePath.getLocalShadowHistoryPath();
	
	// Full path to metadata directory
	hstpath += "/";
	hstpath += filename;
	hstpath += ".hst";

	// create the image history object for the image
	//
	//ImageHistory imageHistory(hstpath.c_str());
	// Add the comment
	char buff[128];
	sprintf_s(buff, 128, "%.4d", ver);

	History &history = History::getHistory();
	//imageHistory.add(filepath, buff, comment, he);
	history.add(filepath, buff, comment, he);

	return true;

}
*/

void ArchiveBuilder::copyBasicExit(MetadataObject& metadataObject, const BasicExif &be) {
	metadataObject.join(be);
}

void ArchiveBuilder::copyExternalExif(MetadataObject& metadataObject, ExifObject& exifObject) {
	metadataObject.join(exifObject);
}

/* AO
bool ArchiveBuilder::showCheckedOut(const char *filepath) {
	CheckoutStatus checkoutStatus;
	if (checkoutStatus.showCheckedOut(filepath) == false) {
		return false;
	}
	return true;
}

bool ArchiveBuilder::showUncheckedOutChanges(const char *filepath) {
	CheckoutStatus checkoutStatus;
	if (checkoutStatus.showUncheckedOutChanges(filepath) == false) {
		return false;
	}
	return true;
}


bool ArchiveBuilder::checkout(const char *filepath, const char *comment) {
	CheckoutStatus checkoutStatus;
	if (checkoutStatus.checkout(filepath) == false) {
		return false;
	}
	try {
		VersionControl &versionControl = VersionControl::get();
		if (versionControl.checkout(filepath, comment) == false) {
			return false;
		}
		if (CheckFile::CheckOut(filepath) == false) {
			return false;
		}
		return true;
	}
	catch (SIAAppException &e) {
		printf("Error: %s\n", e.what());
		if (checkoutStatus.checkin(filepath) == false) {
			return false;
		}
		return false;
	}

	return true;
}

bool ArchiveBuilder::checkin(const char *filepath, const char *comment) {
	CLogger &logger = CLogger::getLogger();
	CheckoutStatus checkoutStatus;
	if (checkoutStatus.checkin(filepath) == false) {
		return false;
	}
	try {
		VersionControl &versionControl = VersionControl::get();
		
		if (versionControl.checkin(filepath, comment) == false) {
			return false;
		}
		if (CheckFile::CheckIn(filepath) == false) {
			return false;
		}
		return true;
	}
	catch (SIAAppException &e) {
		printf("Error: %s\n", e.what());
		if (checkoutStatus.checkin(filepath) == false) {
			return false;
		}
		return false;
	}
	return true;
}

bool ArchiveBuilder::uncheckout(const char *filepath, const char *comment) {

	VersionControl &versionControl = VersionControl::get();
	versionControl.uncheckout(filepath, comment);
	return true;
}
*/
void ArchiveBuilder::print(ExifObject &eo) {
	CLogger &logger = CLogger::getLogger();
	//printf("%s\n", eo.getName().c_str());
	logger.log(LOG_OK, CLogger::FINE, "External Exif");
	MTTableSchema& eos = (MTTableSchema&)eo.getSchema();
	for (std::vector<MTSchema>::iterator i = eos.begin(); i != eos.end(); i++) {
		MTSchema& columnInfo = *i;
		//printf("%-20s %s\n", columnInfo.getName().c_str(), eo.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(LOG_OK, CLogger::FINE, "%-20s %s", columnInfo.getName().c_str(), eo.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}

void ArchiveBuilder::print(const BasicExif &be) {
	CLogger &logger = CLogger::getLogger();
	//printf("%s\n", be.getName().c_str());
	logger.log(LOG_OK, CLogger::FINE, "Basic Exif");
	MTTableSchema& bes = (MTTableSchema&)be.getSchema();
	for (std::vector<MTSchema>::iterator i = bes.begin(); i != bes.end(); i++) {
		MTSchema& columnInfo = *i;
		//printf("%-20s %s\n", columnInfo.getName().c_str(), be.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(LOG_OK, CLogger::FINE, "%-20s %s", columnInfo.getName().c_str(), be.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}

void ArchiveBuilder::print(const MetadataObject& mo) {
	CLogger &logger = CLogger::getLogger();
	//printf("%s\n", mo.getName().c_str());
	logger.log(LOG_OK, CLogger::FINE, "Final metadata");
	MTTableSchema& mos = (MTTableSchema&)mo.getSchema();
	for (std::vector<MTSchema>::iterator i = mos.begin(); i != mos.end(); i++) {
		MTSchema& columnInfo = *i;
		//printf("%-20s %s\n", columnInfo.getName().c_str(), mo.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(LOG_OK, CLogger::FINE, "%-20s %s\n", columnInfo.getName().c_str(), mo.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}
/* AO
bool ArchiveBuilder::exportImages(const char *dispPath) {
	ExportImages exportImages(m_shadowPath.c_str());
	exportImages.process();
	return true;
}

bool ArchiveBuilder::listContents(const char *addressScope) {
	ContentsLister contentsLister(m_shadowPath.c_str());
	contentsLister.List();
	return true;
}
*/
/*
int ArchiveBuilder::revisions(const char *filepath) {
	VersionControl &versionControl = VersionControl::get();
	versionControl.setPathToArchive(m_RootOutputDir);
	return versionControl.getNumberOfVersions(filepath);
}

bool ArchiveBuilder::getImageVersion(const char *imagePath, const char *targetPath, int idx) {
	VersionControl &versionControl = VersionControl::get();
	versionControl.setPathToArchive(m_RootOutputDir);
	return versionControl.getImageVersion(imagePath, targetPath, idx);
}
*/

} /* namespace simplearchive */
