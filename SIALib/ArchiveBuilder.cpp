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
#include "HistoryEvent.h"
#include "History.h"
#include "ImageHistory.h"
#include "CSVDatabase.h"
#include "HookCmd.h"
#include "ViewManager.h"
#include "VersionControl.h"
#include "ImportJournal.h"

#include <stdio.h>
#include <sstream>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

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
				logger.log(CLogger::INFO, "Error processing image not archiving: \"%s\"", curImageSet->getName().c_str());
				return;
			}
			curImageSet->PostProcess();

			m_archiveDate.reset(new ArchiveDate);
			if (m_archiveDate->process(*curImageSet) == false) {
				logger.log(CLogger::INFO, "Error processing image archive date, not archiving: \"%s\"", curImageSet->getName().c_str());
				return;
			}

			ExifDate archiveDate = m_archiveDate->getArchiveDate();
			curImageSet->setTime(archiveDate.getTime());
			m_imagePath.reset(new ImagePath(archiveDate.getTime()));
			
			
			
			curImageSet->PostProcess();
			if (curImageSet->getError() < 0) {
				logger.log(CLogger::INFO, "Error processing image not archiving: \"%s\"", curImageSet->getName().c_str());
				return;
			}

			logger.log(CLogger::INFO, "Image: %s set to archive date: \"%s\"", curImageSet->getName().c_str(), archiveDate.toString().c_str());
			if (curImageSet->getComment().empty() == true) {
				curImageSet->setComment("New Image");
			}

			std::string imageRootPath = curImageSet->getPath();
			logger.log(CLogger::INFO, "Image(s) source path: \"%s\"", imageRootPath.c_str());
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
			int seqNumber = m_csvDBFile.getMaxIndex();
			temp.setSequenceId(seqNumber);
			ExifDateTime timeNow;
			timeNow.now();
			temp.setDateAdded(timeNow);
			// Create metadata xml file for this image.
			if (CreateImage(basicExif, *m_imagePath, m_csvDBFile, temp) == false) {
				return false;
			}
			std::string filepath = m_imagePath->getRelativePath() + '/' + picName;
			std::string shortFilePath = m_imagePath->getYyyymmddStr() + "/" + picName;
			// main
			if (m_imagePath->copyFile(m_curImageSet->getPath(), picName) == false) {
				return false;
			}
			
			

			//std::string shortPath = 
			m_csvDatabase.add(temp, shortFilePath.c_str());

			if (ViewManager::GetInstance().add(seqNumber) == false) {
				return false;
			}

			ImportJournal& importJournal = ImportJournalManager::GetJournal();
			std::string from = path + "/" + picName;
			importJournal.update(from.c_str(), ImportJournal::Imported, shortFilePath.c_str());

			PostArchiveCmd postArchiveCmd(*m_imagePath);
			postArchiveCmd.process();
			logger.log(CLogger::INFO, "Picture image added: \"%s\"", picName);

			

			m_imageIndex.updatePath(basicExif.getCrc(), shortFilePath.c_str());


			const HistoryEvent he(HistoryEvent::ADDED);
			processHistory(*m_imagePath, shortFilePath.c_str(), m_curImageSet->getComment().c_str(), he, 0);
			return true;
		}

		bool CreateImage(const BasicExif &basicExif, ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject) {
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

			if (csvDBFile.insert(seqNumber, imagePath.getRelativePath().c_str(), imagePath.getImageName().c_str(), n, crc, basicExif.getMd5().c_str(),
				basicExif.getUuid().c_str(), 0, date) == false) {
				return false;
			}
			return true;
		}

		bool processHistory(ImagePath &imagePath, const char *filepath, const char *comment, const HistoryEvent &he, int ver) {

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
			/*
			std::string hstpath = path;
			hstpath += "/.sia/history";
			if (SAUtils::DirExists(hstpath.c_str()) == false) {
			if (SAUtils::mkDir(hstpath.c_str()) == false) {
			throw std::exception();
			}
			}
			*/
			// Full path to metadata directory
			hstpath += "/";
			hstpath += filename;
			hstpath += ".hst";

			// create the image history object for the image
			//
			ImageHistory imageHistory(hstpath.c_str());
			// Add the comment
			char buff[128];
			sprintf_s(buff, 128, "%.4d", ver);

			History &history = History::getHistory();
			imageHistory.add(filepath, buff, comment, he);
			history.add(filepath, buff, comment, he);

			return true;

		}

		ImagePath& getImagePath() {
			return *m_imagePath;
		}
	};

	ArchiveBuilder::ArchiveBuilder() {
		m_Error = false;
		m_imageIndex = new ImageIndex;
		m_archiveDate = new ArchiveDate;
		//m_archiveDate->setUseDateToday(true);
		m_useExternalExifTool = false;

	}

	ArchiveBuilder::~ArchiveBuilder() {
		delete m_imageIndex;
		delete m_archiveDate;
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

		if (SAUtils::DirExists(m_shadowPath.c_str()) == false) {
			logger.log(CLogger::FATAL, "Archive Repository directory not accessable? \"%s\"", m_shadowPath.c_str());
			m_Error = true;
		}
		logger.log(CLogger::INFO, "Found Archive Repository directory \"%s\"", m_shadowPath.c_str());


		if (SAUtils::DirExists(m_indexPath.c_str()) == false) {
			logger.log(CLogger::FATAL, "Archive Index directory not accessable? \"%s\"", m_indexPath.c_str());
			m_Error = true;
		}
		logger.log(CLogger::INFO, "Found Archive Index directory \"%s\"", m_indexPath.c_str());


		if (SAUtils::DirExists(m_workspacePath.c_str()) == false) {
			logger.log(CLogger::FATAL, "Secondary Archive Repository directory not accessable? \"%s\"", m_workspacePath.c_str());
			m_Error = true;
		}
		logger.log(CLogger::INFO, "Found Secondary Archive Repository directory \"%s\"", m_workspacePath.c_str());
		m_imageIndex->init(m_indexPath.c_str());
		return (!m_Error);
	}

	bool ArchiveBuilder::Import(const char *sourcePath) {

		CLogger &logger = CLogger::getLogger();
		SACmdArgs &saCmdArgs = SACmdArgs::get();
		CSVDatabase &csvDB = CSVDatabase::get();
		const char *seqPath = ImagePath::getShadowSequenceNumberPath().c_str();
		CSVDBFile csvDBFile(seqPath);

		std::string metatemplatePath = m_metatemplatePath;
		metatemplatePath += "/default.tpl";

		if (MetadataTemplate::read(metatemplatePath.c_str()) == false) {
			logger.log(CLogger::INFO, "Cannot read default template file \"%s\"", metatemplatePath.c_str());
		}
		logger.log(CLogger::INFO, "Read metadata template file \"%s\"", metatemplatePath.c_str());
		// Creating metadata object with bulk data


		//
		// ==== Step 1 ====
		// Read files into folder sets (ImageSets)
		//
		logger.log(CLogger::ERROR, "Stage 1: Placing Image files into folder sets");
		ImportJournal& importJournal = ImportJournalManager::GetJournal();
		TargetsList targetsList;
		targetsList.process(sourcePath);
		
		

		ImageSets *imageSets = 0;
		if ((imageSets = targetsList.getImageSets()) == 0) {
			// No images to process
			return false;
		}
		imageSets->processImportJournal(importJournal);
		imageSets->processHook();

		// process folder sets into image groups
		ImageGroups imageGroups;


		if (imageSets == 0) {
			return false;
		}
		logger.log(CLogger::SUMMARY, "Found %d Folders and %d files to be processed", TargetsList::getFolderCount(), TargetsList::getFileCount());
		//
		// ==== Step 2 ====
		// Read files into Image group sets (ImageGroups)
		//
		ArchiveRepository &archiveRepository = ArchiveRepository::get();
		archiveRepository.setPathToArchive(m_shadowPath);
		logger.log(CLogger::ERROR, "Stage 2: Processing Image files");
		archiveRepository.setPathToActiveRoot(this->m_workspacePath);
		/// Iterate the Image Sets
		for (std::vector<ImageSet *>::iterator i = imageSets->begin(); i != imageSets->end(); i++) {
			ImageSet *imageSet = *i;

			/// Create a Image Group container
			ImageGroup *imageGroup = new ImageGroup(imageSet->getPath());
			/// Insert into Image Group Sets
			imageGroups.insert(imageGroups.end(), imageGroup);
			logger.log(CLogger::SUMMARY, "Processing Image files in the source location %s", imageSet->getPath());
			/// Iterate the current image set
			for (std::vector<ImageItem *>::iterator i = imageSet->begin(); i != imageSet->end(); i++) {
				/// Image Item
				ImageItem *imageItem = *i;
				logger.log(CLogger::SUMMARY, "Processing Image file: %s", imageItem->getFilename().c_str());
				ExifObject *exifObject = nullptr;
				//data->print();	
				bool status = false;
				std::string path = imageItem->getPath();
				if (SAUtils::IsFile(imageItem->getPath()) == false) {
					continue;
				}
				ImageExtentions& ie = ImageExtentions::get();
				if (ie.IsValid(imageItem->getFilename().c_str()) == false) {
					logger.log(CLogger::WARNING, "Not a valid file type \"%s\" rejecting ", imageItem->getFilename().c_str());
					continue;
				}
				//logger.log(CLogger::SUMMARY, ": %s", imageItem->getFilename().c_str());

				std::string pathstr = imageItem->getPath();
				logger.log(CLogger::INFO, "Path of image: %s", pathstr.c_str());
				// This needs to be replaced with a factory class
				// ImageId object reads the image byte by byte for md5 and crc and Basic Exif.


				BasicExif_Ptr basicExifPtr = BasicExifFactory::make(pathstr);
				const BasicExif& basicExif = *basicExifPtr;
				print(basicExif);
				// Add Image to the Index database if not a dup.
				logger.log(CLogger::INFO, "Find if dup %s", imageItem->getFilename().c_str());
				// If Make no changes == true, do not add this image into the dups index
				if (!saCmdArgs.IsMakeNoChanges()) { // DEBUG only allow dups
					// Force changes i.e the archive is damaged and images are being
					// added from a backup
					if (saCmdArgs.IsForceChanges()) {
						if (m_imageIndex->IsDup(basicExif.getCrc())) {
							//m_imageIndex->getData(basicExif.getCrc());
							logger.log(CLogger::INFO, "Dup %s", imageItem->getFilename().c_str());
							// reject image from import
							continue;
						}
					}
					else {
						// normal operation. dups are rejected
						int pos = -1;
						if (pos = m_imageIndex->IsDup(basicExif.getCrc())) {
							//m_imageIndex->getData(imageId->getCrc());
							logger.log(CLogger::WARNING, "    Image \"%s\" was found to be a duplicate. Rejecting from import", imageItem->getFilename().c_str());
							// reject image from import
							ImageId imageId = m_imageIndex->findDup(basicExif.getCrc());
							if (imageId.getName().empty()) {
								logger.log(CLogger::ERROR, "Image indexing corrupt %s", imageItem->getFilename().c_str());
							}
							else {
								importJournal.update(imageItem->getPath(), ImportJournal::Duplicate, imageId.getLocation().c_str());
							}


							continue;
						}
						else {
							// Add To the Image Indexing (used to find duplicates)
							if (m_imageIndex->add(basicExif) == false) {
								logger.log(CLogger::WARNING, "    Image \"%s\" was found to be a duplicate. Rejecting from import", imageItem->getFilename().c_str());
								// reject image from import
								ImageId imageId = m_imageIndex->findDup(basicExif.getCrc());
								if (imageId.getName().empty()) {
									logger.log(CLogger::ERROR, "Image indexing corrupt %s", imageItem->getFilename().c_str());
								}
								else {
									importJournal.update(imageItem->getPath(), ImportJournal::Duplicate, imageId.getLocation().c_str());
								}
								continue;
							}
							logger.log(CLogger::INFO, "Adding image to dups index %s", imageItem->getFilename().c_str());
						}
					}
				}
				// Not a dup so add to group. 
				if (!basicExif.isExifFound()) {
					logger.log(CLogger::INFO, "No EXIF simple EXIF infomation found in \"%s\"", imageItem->getFilename().c_str());
				}
				logger.log(CLogger::INFO, "EXIF simple EXIF infomation found in \"%s\"", imageItem->getFilename().c_str());
				if (!basicExif.isExifFound() || m_useExternalExifTool) {
					// Try external Exif Tool
					logger.log(CLogger::INFO, "Using external EXIF tool on file \"%s\"", imageItem->getFilename().c_str());
					exifObject = ImageFileReader::externalExifTool(pathstr);
				}
				//exifObject->debugPrint();
				if (exifObject == nullptr) {
					logger.log(CLogger::INFO, "External EXIF reader not installed. No extra EXIF read for image\"%s\"", imageItem->getFilename().c_str());
				}
				else {
					logger.log(CLogger::INFO, "External EXIF reader completed for image\"%s\"", imageItem->getFilename().c_str());
					print(*exifObject);
				}
				logger.log(CLogger::INFO, "Processing bulk metedata %s", imageItem->getFilename().c_str());

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
				logger.log(CLogger::INFO, "completed step2 \"%s\"", imageItem->getFilename().c_str());
				/*
				XMLWriter xmlWriter;
				xmlWriter.writeImage(*metadataObject, "c:/temp/image.xml");
				*/
			}

		}

		
		// imageSets are no longer required so can be deleted.
		targetsList.destroy();

		for (std::vector<ImageGroup *>::iterator i = imageGroups.begin(); i != imageGroups.end(); i++) {
			ImageGroup *group = *i;
			for (std::vector<ImageContainer *>::iterator i = group->begin(); i != group->end(); i++) {
				ImageContainer *imageSet = *i;
				ImageProcessor imageProcessor(imageSet, csvDBFile, csvDB, m_workspacePath, *m_imageIndex);
				
				/*
				imageSet->PostProcess();
				if (imageSet->getError() < 0) {
					logger.log(CLogger::INFO, "Error processing image not archiving: \"%s\"", imageSet->getName().c_str());
					continue;
				}

				logger.log(CLogger::SUMMARY, "Creating Metadata for Image: \"%s\"", imageSet->getName().c_str());
				if (m_archiveDate->process(*imageSet) == false) {
					logger.log(CLogger::INFO, "Error processing image archive date, not archiving: \"%s\"", imageSet->getName().c_str());
					continue;
				}
				ExifDate archiveDate = m_archiveDate->getArchiveDate();
				imageSet->setTime(archiveDate.getTime());
				ImagePath imagePath(archiveDate.getTime());

				logger.log(CLogger::INFO, "Image: %s set to archive date: \"%s\"", imageSet->getName().c_str(), archiveDate.toString().c_str());
				if (imageSet->getComment().empty() == true) {
					imageSet->setComment("New Image");
				}

				std::string imageRootPath = imageSet->getPath();
				logger.log(CLogger::INFO, "Image(s) source path: \"%s\"", imageRootPath.c_str());
				imageSet->setImageRootPath(imagePath.getRelativePath());

				const BasicExifFactory *imageId;
				MetadataObject *metadataObject;
				if (imageSet->hasPictureFile()) {
					const BasicExif &basicExif = imageSet->getPictureId();
					MetadataObject *metadataObject = (MetadataObject *)imageSet->getPictureMetadata();
					std::string picName = imageSet->getPictureFile();
					
					
					imagePath.setImageName(picName.c_str());
					int seqNumber = csvDBFile.getMaxIndex();
					metadataObject->setSequenceId(seqNumber);
					ExifDateTime timeNow;
					timeNow.now();
					metadataObject->setDateAdded(timeNow);
					// Create metadata xml file for this image.
					if (CreateImage(basicExif, imagePath, csvDBFile, *((MetadataObject *)metadataObject)) == false) {
						return false;
					}




					std::string relImagePath = imagePath.getYyyymmddStr() + "/" + imageSet->getPictureFile();


					const MetadataObject *picMetadata = imageSet->getPictureMetadata();
					MetadataObject &temp = (MetadataObject &)*picMetadata;
					// main
					if (imagePath.copyFile(imageSet->getPath(), imageSet->getPictureFile()) == false) {
						return false;
					}
					//std::string shortPath = 
					csvDB.add(temp, relImagePath.c_str());

					if (ViewManager::GetInstance().add(seqNumber) == false) {
						return false;
					}

					PostArchiveCmd postArchiveCmd(imagePath);
					postArchiveCmd.process();
					logger.log(CLogger::INFO, "Picture image added: \"%s\"", imageSet->getPictureFile());

					std::string filepath = imagePath.getRelativePath() + '/' + imageSet->getPictureFile();
					const HistoryEvent he(HistoryEvent::ADDED);
					processHistory(imagePath, filepath.c_str(), imageSet->getComment().c_str(), he, 0);
					
				}
				if (imageSet->hasRawFile()) {
					const BasicExif &basicExif = imageSet->getRawId();
					metadataObject = (MetadataObject *)imageSet->getRawMetadata();
					std::string rawPath = imageSet->getRawFile();
					imagePath.setImageName(rawPath.c_str());

					if (CreateImage(basicExif, imagePath, csvDBFile, *((MetadataObject *)metadataObject)) == false) {
						return false;
					}

					// Create metadata xml file for this image.
					//if (CreateMetadataXMLFile(imagePath, csvDBFile, *((MetadataObject *)metadataObject)) == false) {
					//	return false;
					//}
					//					if (m_viewManager->add2Master(imagePath.getRelativePath().c_str(), imageSet->getRawFile()) == false) {
					//						return false;
					//					}

				}

*/
			}
			
			/*
			if (!saCmdArgs.IsMakeNoChanges()) {
				// process ImageSet
				logger.log(CLogger::SUMMARY, "Starting Archiving phase");
				archiveRepository.Import(*group);
			}
			*/

		}
		ImportJournalManager::save();
		_CrtDumpMemoryLeaks();
		return true;
	}





	

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

	if (csvDBFile.insert(seqNumber, imagePath.getRelativePath().c_str(), imagePath.getImageName().c_str(), n, crc, basicExif.getMd5().c_str(),
																									basicExif.getUuid().c_str(), 0, date) == false) {
		return false;
	}
	return true;
}
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
	/*
	std::string hstpath = path;
	hstpath += "/.sia/history";
	if (SAUtils::DirExists(hstpath.c_str()) == false) {
		if (SAUtils::mkDir(hstpath.c_str()) == false) {
			throw std::exception();
		}
	}
	*/
	// Full path to metadata directory
	hstpath += "/";
	hstpath += filename;
	hstpath += ".hst";

	// create the image history object for the image
	//
	ImageHistory imageHistory(hstpath.c_str());
	// Add the comment
	char buff[128];
	sprintf_s(buff, 128, "%.4d", ver);

	History &history = History::getHistory();
	imageHistory.add(filepath, buff, comment, he);
	history.add(filepath, buff, comment, he);

	return true;

}

void ArchiveBuilder::copyBasicExit(MetadataObject& metadataObject, const BasicExif &be) {
	metadataObject.join(be);
}

void ArchiveBuilder::copyExternalExif(MetadataObject& metadataObject, ExifObject& exifObject) {
	metadataObject.join(exifObject);
}

bool ArchiveBuilder::checkout(const char *filepath, const char *comment) {

	try {
		VersionControl &versionControl = VersionControl::get();
		return versionControl.checkout(filepath, comment);
	}
	catch (SIAAppException &e) {
		printf("Error: %s\n", e.what());
		return false;
	}
}

bool ArchiveBuilder::checkin(const char *filepath, const char *comment) {

	VersionControl &versionControl = VersionControl::get();
	versionControl.checkin(filepath, comment);
	return true;
}

bool ArchiveBuilder::uncheckout(const char *filepath, const char *comment) {

	VersionControl &versionControl = VersionControl::get();
	versionControl.uncheckout(filepath, comment);
	return true;
}

void ArchiveBuilder::print(ExifObject &eo) {
	CLogger &logger = CLogger::getLogger();
	//printf("%s\n", eo.getName().c_str());
	logger.log(CLogger::FINE, "External Exif");
	MTTableSchema& eos = (MTTableSchema&)eo.getSchema();
	for (std::vector<MTSchema>::iterator i = eos.begin(); i != eos.end(); i++) {
		MTSchema& columnInfo = *i;
		//printf("%-20s %s\n", columnInfo.getName().c_str(), eo.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(CLogger::FINE, "%-20s %s", columnInfo.getName().c_str(), eo.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}

void ArchiveBuilder::print(const BasicExif &be) {
	CLogger &logger = CLogger::getLogger();
	//printf("%s\n", be.getName().c_str());
	logger.log(CLogger::FINE, "Basic Exif");
	MTTableSchema& bes = (MTTableSchema&)be.getSchema();
	for (std::vector<MTSchema>::iterator i = bes.begin(); i != bes.end(); i++) {
		MTSchema& columnInfo = *i;
		//printf("%-20s %s\n", columnInfo.getName().c_str(), be.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(CLogger::FINE, "%-20s %s", columnInfo.getName().c_str(), be.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}

void ArchiveBuilder::print(const MetadataObject& mo) {
	CLogger &logger = CLogger::getLogger();
	//printf("%s\n", mo.getName().c_str());
	logger.log(CLogger::FINE, "Final metadata");
	MTTableSchema& mos = (MTTableSchema&)mo.getSchema();
	for (std::vector<MTSchema>::iterator i = mos.begin(); i != mos.end(); i++) {
		MTSchema& columnInfo = *i;
		//printf("%-20s %s\n", columnInfo.getName().c_str(), mo.columnAt(columnInfo.getName().c_str()).toString().c_str());
		logger.log(CLogger::FINE, "%-20s %s\n", columnInfo.getName().c_str(), mo.columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
}

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
