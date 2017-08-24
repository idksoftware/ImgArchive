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
#include "BasicMetadataFactory.h"
#include "XMLWriter.h"
#include "ImagePath.h"
#include "CSVDBFile.h"
#include "SIAApplicationState.h"
#include "CSVDatabase.h"
#include "HookCmd.h"
#include "ViewManager.h"
//#include "VersionControl.h"
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
#ifdef _WIN32
#include <crtdbg.h>
#else
#include <linux/limits.h>
#define MAX_PATH PATH_MAX
#endif
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
				logger.log(LOG_OK, CLogger::Level::WARNING, "Not a valid file type \"%s\" rejecting ", file.c_str());
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
		
		//CSVDBFile &m_csvDBFile;
		CSVDatabase &m_csvDatabase;
		std::unique_ptr<ImagePath> m_imagePath;
		ImageContainer *m_curImageSet;
		std::unique_ptr<ArchiveDate> m_archiveDate;
		std::string& m_workspacePath;
		//PrimaryIndex& m_imageIndex;
		ArchiveObject &m_archiveObject;
	public:
	
		ImageProcessor(ImageContainer *curImageSet, std::string& workspacePath, ArchiveObject& archiveObject) :
			//m_csvDBFile(csvDBFile),
			m_curImageSet(curImageSet),
			m_workspacePath(workspacePath),
			m_archiveObject(archiveObject),
			m_csvDatabase(CSVDatabase::get())
		{}

		bool Process() {
			CLogger &logger = CLogger::getLogger();
			m_csvDatabase = CSVDatabase::get();

			if (m_curImageSet->getError() < 0) {
				logger.log(LOG_OK, CLogger::Level::ERR, "Error processing image not archiving: \"%s\"", m_curImageSet->getName().c_str());
				return false;
			}
			m_curImageSet->PostProcess();

			m_archiveDate.reset(new ArchiveDate);
			if (m_archiveDate->process(*m_curImageSet) == false) {
				logger.log(LOG_OK, CLogger::Level::ERR, "Error processing image capture date, not found: \"%s\"", m_curImageSet->getName().c_str());
			}
			// using another date
			ExifDate archiveDate = m_archiveDate->getArchiveDate();
			m_curImageSet->setTime(archiveDate.getTime());
			m_imagePath = std::make_unique<ImagePath>(archiveDate.getTime());
			
			if (m_curImageSet->getError() < 0) {
				logger.log(LOG_OK, CLogger::Level::ERR, "Error processing image not archiving: \"%s\"", m_curImageSet->getName().c_str());
				return false;
			}

			logger.log(LOG_OK, CLogger::Level::INFO, "Image: %s set to archive date: \"%s\"", m_curImageSet->getName().c_str(), archiveDate.toString().c_str());
			if (m_curImageSet->getComment().empty() == true) {
				m_curImageSet->setComment("New Image");
			}


			std::string imageRootPath = m_curImageSet->getPath();
			logger.log(LOG_OK, CLogger::Level::INFO, "Image(s) source path: \"%s\"", imageRootPath.c_str());
			m_curImageSet->setImageRootPath(m_imagePath->getRelativePath());
			if (m_curImageSet->hasPictureFile()) {
				const BasicMetadata &BasicMetadata = m_curImageSet->getPictureId();
				const MetadataObject &metadataObject = *m_curImageSet->getPictureMetadata();
				std::string picName = m_curImageSet->getPictureFile();
				std::string path = m_curImageSet->getPath();
				if (processImage(picName, path, metadataObject, BasicMetadata) == false) {
					return false;
				}
				
			}
			if (m_curImageSet->hasRawFile()) {
				const BasicMetadata &BasicMetadata = m_curImageSet->getRawId();
				const MetadataObject &metadataObject = *m_curImageSet->getRawMetadata();
				std::string picName = m_curImageSet->getRawFile();
				std::string path = m_curImageSet->getPath();
				if (processImage(picName, path, metadataObject, BasicMetadata) == false) {
					return false;
				}
				
			}
			return true;
		}

		bool processImage(std::string& picName, std::string& path, const MetadataObject &metadataObject, const BasicMetadata &BasicMetadata) {

			MetadataObject &temp = (MetadataObject &)metadataObject;
			CLogger &logger = CLogger::getLogger();
		
			m_imagePath->setImageName(picName.c_str());
			m_imagePath->setCurrentSourcePath(m_curImageSet->getPath().c_str());
			
			if (m_archiveObject.CreateImage(BasicMetadata, *m_imagePath, temp) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to import the image: \"%s\"", picName.c_str());
				return false;
			}
			std::string shortFilePath = m_imagePath->getYyyymmddStr() + "/" + m_imagePath->getImageName();
			
			//std::string shortPath = 
			//m_csvDatabase.add(temp, shortFilePath.c_str());
			
			//if (ViewManager::GetInstance().add(seqNumber) == false) {
			//	return false;
			//}
			
			ImportJournal& importJournal = ImportJournalManager::GetJournal();
			std::string from = path + "/" + picName;
			importJournal.update(from.c_str(), ImportJournal::Imported, shortFilePath.c_str());

			PostArchiveCmd postArchiveCmd(*m_imagePath);
			postArchiveCmd.process();
			logger.log(LOG_OK, CLogger::Level::SUMMARY, "Archiving image: \"%s\"", picName.c_str());
	
			processHistory(*m_imagePath, m_curImageSet->getComment().c_str());



			return true;
		}


		/*
		bool CreateImage(const BasicMetadata &BasicMetadata, ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject) {
			
			return ArchiveObject::get().CreateImage(BasicMetadata, imagePath, csvDBFile, metadataObject);
		}
		*/

		bool processHistory(ImagePath &imagePath, const char *comment) {
			
			return ArchiveObject::getInstance().processHistory(imagePath, comment);
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
		
		m_MasterPath = m_archiveObject.getMasterPath().getRepositoryPath();
		if (m_MasterPath.empty()) {
			return false;
		}
		m_indexPath = m_archiveObject.getPrimaryIndexObject().getPrimaryIndexPath().getImageIndexPath();
		if (m_indexPath.empty()) {
			return false;
		}
		m_workspacePath = config.getWorkspacePath();
		if (m_workspacePath.empty()) {
			return false;
		}
		m_metatemplatePath = config.getMetadataTemplatePath();
		if (m_metatemplatePath.empty()) {
			return false;
		}
		std::string MasterJournalPath = ImagePath::getMasterJournalPath();
		ImportJournalManager::setJournalFilePath(MasterJournalPath.c_str());

		ViewManager::GetInstance().initaliseMaster(config.getWorkspacePath(), config.getMasterViewPath());

		CLogger &logger = CLogger::getLogger();

		logger.log(LOG_OK, CLogger::Level::SUMMARY, "Using Master folder: \"%s\"", m_MasterPath.c_str());
		if (SAUtils::DirExists(m_MasterPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Archive Repository directory not accessable? \"%s\"", m_MasterPath.c_str());
			m_Error = true;
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Found Archive Repository directory \"%s\"", m_MasterPath.c_str());


		if (SAUtils::DirExists(m_indexPath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Archive Index directory not accessable? \"%s\"", m_indexPath.c_str());
			m_Error = true;
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Found Archive Index directory \"%s\"", m_indexPath.c_str());

		logger.log(LOG_OK, CLogger::Level::SUMMARY, "Using Workspace folder: \"%s\"", m_MasterPath.c_str());
		if (SAUtils::DirExists(m_workspacePath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Workspace directory not accessable? \"%s\"", m_workspacePath.c_str());
			m_Error = true;
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Found Workspace Archive Repository directory \"%s\"", m_workspacePath.c_str());
		// Primary Index path
		PrimaryIndexObject& primaryIndexObject = m_archiveObject.getPrimaryIndexObject();
		ImageIndex& imageIndex = primaryIndexObject.getimageIndex();
		imageIndex.init(m_indexPath.c_str());
		
	
		std::string MasterPath = ImagePath::getMasterHistoryPath();
		
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
			logger.log(LOG_OK, CLogger::Level::ERR, "Source path \"%s\" not Found", sourcePath);
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
		logger.log(LOG_ANALISING, CLogger::Level::INFO, "Stage 1: Placing Image files into folder sets");
		
		TargetsList targetsList;
		targetsList.process(sourcePath);

		logger.log(LOG_IMAGE_SUMMARY, CLogger::Level::SUMMARY, "%d files to be processed", TargetsList::getFileCount());
		logger.log(LOG_FOLDER_SUMMARY, CLogger::Level::SUMMARY, "%d Folders to be processed", TargetsList::getFolderCount());
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
		
		const char *seqPath = ImagePath::getMasterSequenceNumberPath().c_str();
		
		PrimaryIndexObject& primaryIndexObject = m_archiveObject.getPrimaryIndexObject();
		ImageIndex& imageIndex = primaryIndexObject.getimageIndex();

		std::string metatemplatePath = m_metatemplatePath;
		metatemplatePath += "/default.tpl";

		if (MetadataTemplate::read(metatemplatePath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::INFO, "Cannot read default template file \"%s\"", metatemplatePath.c_str());
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Read metadata template file \"%s\"", metatemplatePath.c_str());
		// Creating metadata object with bulk data

		imageSets->processImportJournal(importJournal);
		imageSets->processHook();
		logger.log(LOG_OK, CLogger::Level::INFO, "Found %d images to be processed", importJournal.numberOfImages());
		ImportJournalManager::save();
		// process folder sets into image groups
		//ImageGroups imageGroups;


		if (imageSets == nullptr) {
			return false;
		}
		// ==== Step 2 ====
		// Read files into Image group sets (ImageGroups)
		//
		ArchiveRepository &archiveRepository = ArchiveRepository::get();
		archiveRepository.setPathToArchive(m_MasterPath);
		logger.log(LOG_IMPORTING, CLogger::Level::ERR, "Stage 2: Processing Image files");
		archiveRepository.setPathToActiveRoot(this->m_workspacePath);
		/// Iterate the Image Sets
		
		for (std::vector<ImageSet *>::iterator i = imageSets->begin(); i != imageSets->end(); i++) {
			ImageSet *imageSet = *i;

			/// Create a Image Group container
			ImageGroup *imageGroup = new ImageGroup(imageSet->getPath());
			/// Insert into Image Group Sets
			//imageGroups.insert(imageGroups.end(), imageGroup);
			logger.log(LOG_OK, CLogger::Level::SUMMARY, "Processing Image files in the source location %s", imageSet->getPath());
			/// Iterate the current image set
			for (std::vector<ImageItem *>::iterator i = imageSet->begin(); i != imageSet->end(); i++) {
				/// Image Item
				SIAApplicationState::RunState state = SIAApplicationState::getState();
				ImageItem *imageItem = *i;
				importJournal.setCurrent(imageItem->getPath());
				logger.log(LOG_CURRENT_IMAGE, CLogger::Level::SUMMARY, "%d image\n", importJournal.getCurrentImageIndex()+1);
				logger.log(LOG_IMAGE_NAME, CLogger::Level::SUMMARY, "%s", imageItem->getFilename().c_str());
				logger.log(LOG_OK, CLogger::Level::SUMMARY, "Processing Image file: %s", imageItem->getFilename().c_str());
				ExifObject *exifObject = nullptr;
				//data->print();	
				bool status = false;
				std::string path = imageItem->getPath();
				if (SAUtils::IsFile(imageItem->getPath()) == false) {
					continue;
				}
				ImageExtentions& ie = ImageExtentions::get();
				if (ie.IsValid(imageItem->getFilename().c_str()) == false) {
					logger.log(LOG_INVALID_FILE_TYPE, CLogger::Level::WARNING, "Not a valid file type \"%s\" rejecting ", imageItem->getFilename().c_str());
					continue;
				}
				//logger.log(LOG_OK, CLogger::SUMMARY, ": %s", imageItem->getFilename().c_str());

				std::string pathstr = imageItem->getPath();
				logger.log(LOG_OK, CLogger::Level::INFO, "Path of image: %s", pathstr.c_str());
				// This needs to be replaced with a factory class
				// ImageId object reads the image byte by byte for md5 and crc and Basic Exif.


				BasicMetadata_Ptr BasicMetadataPtr = BasicMetadataFactory::make(pathstr);
				BasicMetadataPtr->print();
				print(*BasicMetadataPtr);
				const BasicMetadata& BasicMetadata = *BasicMetadataPtr;
				print(BasicMetadata);
				// Add Image to the Index database if not a dup.
				logger.log(LOG_OK, CLogger::Level::INFO, "Find if dup %s", imageItem->getFilename().c_str());
				// If Make no changes == true, do not add this image into the dups index
				if (!saCmdArgs.IsMakeNoChanges()) { // DEBUG only allow dups
					// Force changes i.e the archive is damaged and images are being
					// added from a backup
					if (saCmdArgs.IsForceChanges()) {
						if (imageIndex.IsDup(BasicMetadata.getCrc())) {
							//m_imageIndex->getData(BasicMetadata.getCrc());
							logger.log(LOG_OK, CLogger::Level::INFO, "Dup %s", imageItem->getFilename().c_str());
							// reject image from import
							continue;
						}
					}
					else {
						// normal operation. dups are rejected
						int pos = -1;
						if (pos = imageIndex.IsDup(BasicMetadata.getCrc())) {
							//m_imageIndex->getData(imageId->getCrc());
							logger.log(LOG_DUPLICATE, CLogger::Level::WARNING, "    Image \"%s\" was found to be a duplicate. Rejecting from import", imageItem->getFilename().c_str());
							// reject image from import
							ImageId imageId = imageIndex.findDup(BasicMetadata.getCrc());
							if (imageId.getName().empty()) {
								logger.log(LOG_OK, CLogger::Level::ERR, "Image indexing corrupt %s", imageItem->getFilename().c_str());
							}
							else {
								importJournal.update(imageItem->getPath(), ImportJournal::Duplicate, imageId.getLocation().c_str());
								if (ImportJournalManager::save() == false) {
									logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
									return false;
								}
							}
							continue;
						}
						else {
							// Add To the Image Indexing (used to find duplicates)
							if (imageIndex.add(BasicMetadata) == false) {
								logger.log(LOG_DUPLICATE, CLogger::Level::WARNING, "    Image \"%s\" was found to be a duplicate. Rejecting from import", imageItem->getFilename().c_str());
								// reject image from import
								ImageId imageId = imageIndex.findDup(BasicMetadata.getCrc());
								if (imageId.getName().empty()) {
									logger.log(LOG_OK, CLogger::Level::ERR, "Image indexing corrupt %s", imageItem->getFilename().c_str());
								}
								else {
									importJournal.update(imageItem->getPath(), ImportJournal::Duplicate, imageId.getLocation().c_str());
									if (ImportJournalManager::save() == false) {
										logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
										return false;
									}
								}
								continue;
							}
							logger.log(LOG_OK, CLogger::Level::INFO, "Adding image to dups index %s", imageItem->getFilename().c_str());
						}
					}
				}
				// Not a dup so add to group. 
				if (!BasicMetadata.isExifFound()) {
					logger.log(LOG_OK, CLogger::Level::INFO, "No EXIF simple EXIF infomation found in \"%s\"", imageItem->getFilename().c_str());
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "EXIF simple EXIF infomation found in \"%s\"", imageItem->getFilename().c_str());
				if (!BasicMetadata.isExifFound() && m_useExternalExifTool) {
					// Try external Exif Tool
					logger.log(LOG_OK, CLogger::Level::INFO, "Using external EXIF tool on file \"%s\"", imageItem->getFilename().c_str());
					exifObject = ImageFileReader::externalExifTool(pathstr);
				}
				//exifObject->debugPrint();
				if (m_useExternalExifTool == true) {
					if (exifObject == nullptr) {
						logger.log(LOG_OK, CLogger::Level::INFO, "External EXIF reader failed. No extra EXIF read for image\"%s\"", imageItem->getFilename().c_str());
					}
					else {
						logger.log(LOG_OK, CLogger::Level::INFO, "External EXIF reader completed for image\"%s\"", imageItem->getFilename().c_str());
						print(*exifObject);
					}
				}
				else {
					logger.log(LOG_OK, CLogger::Level::INFO, "External EXIF reader not enabled\"%s\"", imageItem->getFilename().c_str());
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "Processing bulk metedata %s", imageItem->getFilename().c_str());

				MetadataTemplate &metadataTemplate = MetadataTemplate::GetInstance();
				MetadataObject_ptr metadataObjectPtr = metadataTemplate.getMetadataObject();
				MetadataObject &metadataObject = *metadataObjectPtr;
				try {
					copyBasicExit(metadataObject, BasicMetadata);
				}
				catch (std::out_of_range &e) {
					printf("%s\n", e.what());
				}
				if (exifObject != nullptr) {
					copyExternalExif(metadataObject, *exifObject);
				}
				print(metadataObject);
				imageGroup->add(BasicMetadataPtr, metadataObjectPtr);
				logger.log(LOG_OK, CLogger::Level::INFO, "completed step2 \"%s\"", imageItem->getFilename().c_str());
				
				//XMLWriter xmlWriter;
				//xmlWriter.writeImage(*metadataObject, "c:/temp/image.xml");
				
			}
			for (std::vector<ImageContainer *>::iterator i = imageGroup->begin(); i != imageGroup->end(); i++) {
				ImageContainer *imageSet = *i;
				
				ImageProcessor imageProcessor(imageSet, m_workspacePath, m_archiveObject);
				if (imageProcessor.Process() == false) {
					return false;
				}
				
			}
		}

		ImportJournalManager::save();
#ifdef _WIN32
		_CrtDumpMemoryLeaks();
#endif
		
		return true;
	}




void ArchiveBuilder::copyBasicExit(MetadataObject& metadataObject, const BasicMetadata &be) {
	metadataObject.join(be);
}

void ArchiveBuilder::copyExternalExif(MetadataObject& metadataObject, ExifObject& exifObject) {
	metadataObject.join(exifObject);
}



void ArchiveBuilder::print(ExifObject &eo) {
	CLogger &logger = CLogger::getLogger();
	//printf("%s\n", eo.getName().c_str());
	logger.log(LOG_OK, CLogger::Level::FINE, "External Exif");
	MTTableSchema& eos = (MTTableSchema&)eo.getSchema();
	for (std::vector<MTSchema>::iterator i = eos.begin(); i != eos.end(); i++) {
		MTSchema& columnInfo = *i;
		//printf("%-20s %s\n", columnInfo.getName().c_str(), eo.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(LOG_OK, CLogger::Level::FINE, "%-20s %s", columnInfo.getName().c_str(), eo.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}

void ArchiveBuilder::print(const BasicMetadata &be) {
	CLogger &logger = CLogger::getLogger();
	printf("%s\n", be.getName().c_str());
	logger.log(LOG_OK, CLogger::Level::FINE, "Basic Exif");
	MTTableSchema& bes = (MTTableSchema&)be.getSchema();
	for (std::vector<MTSchema>::iterator i = bes.begin(); i != bes.end(); i++) {
		MTSchema& columnInfo = *i;
		printf("%-20s %s\n", columnInfo.getName().c_str(), be.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(LOG_OK, CLogger::Level::FINE, "%-20s %s", columnInfo.getName().c_str(), be.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}

void ArchiveBuilder::print(const MetadataObject& mo) {
	CLogger &logger = CLogger::getLogger();
	printf("%s\n", mo.getName().c_str());
	logger.log(LOG_OK, CLogger::Level::FINE, "Final metadata");
	MTTableSchema& mos = (MTTableSchema&)mo.getSchema();
	for (std::vector<MTSchema>::iterator i = mos.begin(); i != mos.end(); i++) {
		MTSchema& columnInfo = *i;
		printf("%-20s %s\n", columnInfo.getName().c_str(), mo.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(LOG_OK, CLogger::Level::FINE, "%-20s %s\n", columnInfo.getName().c_str(), mo.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}




} /* namespace simplearchive */
