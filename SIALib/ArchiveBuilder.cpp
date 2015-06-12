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

#include <stdio.h>
#include <sstream>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


namespace simplearchive {



	ArchiveBuilder::ArchiveBuilder() {
		m_Error = false;
		m_imageIndex = new ImageIndex;
		m_archiveDate = new ArchiveDate;
		//m_archiveDate->setUseDateToday(true);
		m_useExternalExifTool = false;
		m_viewManager = new ViewManager;
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
		m_archivePath = config.getArchivePath();
		m_metatemplatePath = config.getMetadataTemplatePath();
		m_viewManager->initaliseMaster(config.getArchivePath(), config.getMasterViewPath());

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


		if (SAUtils::DirExists(m_archivePath.c_str()) == false) {
			logger.log(CLogger::FATAL, "Secondary Archive Repository directory not accessable? \"%s\"", m_archivePath.c_str());
			m_Error = true;
		}
		logger.log(CLogger::INFO, "Found Secondary Archive Repository directory \"%s\"", m_archivePath.c_str());
		m_imageIndex->init(m_indexPath.c_str());
		return (!m_Error);
	}

	bool ArchiveBuilder::Import(const char *sourcePath) {
		SACmdArgs &saCmdArgs = SACmdArgs::get();
		CSVDatabase &csvDB = CSVDatabase::get();
		//
		// ==== Step 1 ====
		// Read files into folder sets (ImageSets)
		//
		TargetsList targetsList;
		targetsList.process(sourcePath);
		const char *seqPath = ImagePath::getShadowSequenceNumberPath().c_str();
		CSVDBFile csvDBFile(seqPath);

		ImageSets *imageSets = 0;
		if ((imageSets = targetsList.getImageSets()) == 0) {
			// No images to process
			return false;
		}
		//imageSets->print();
		imageSets->processHook();

		// process folder sets into image groups
		ImageGroups imageGroups = ImageGroups();
		CLogger &logger = CLogger::getLogger();

		if (imageSets == 0) {
			return false;
		}
		//
		// ==== Step 2 ====
		// Read files into Image group sets (ImageGroups)
		//
		ArchiveRepository &archiveRepository = ArchiveRepository::get();
		archiveRepository.setPathToArchive(m_shadowPath);

		archiveRepository.setPathToActiveRoot(this->m_archivePath);
		/// Iterate the Image Sets
		for (std::vector<ImageSet *>::iterator i = imageSets->begin(); i != imageSets->end(); i++) {
			ImageSet *imageSet = *i;

			/// Create a Image Group container
			ImageGroup *imageGroup = new ImageGroup(imageSet->getPath());
			/// Insert into Image Group Sets
			imageGroups.insert(imageGroups.end(), imageGroup);
			logger.log(CLogger::ERROR, "Processing set %s", imageSet->getPath());
			/// Iterate the current image set
			for (std::vector<ImageItem *>::iterator i = imageSet->begin(); i != imageSet->end(); i++) {
				/// Image Item
				ImageItem *imageItem = *i;
				ExifObject *exifObject = nullptr;
				//data->print();	
				bool status = false;
				std::string path = imageItem->getPath();
				if (SAUtils::IsFile(imageItem->getPath()) == false) {
					continue;
				}
				ImageExtentions ie = ImageExtentions::get();
				if (ie.IsValid(imageItem->getFilename().c_str()) == false) {
					logger.log(CLogger::INFO, "Not a valid file type \"%s\" rejecting ", imageItem->getFilename().c_str());
					continue;
				}
				logger.log(CLogger::SUMMARY, "Analysing: %s", imageItem->getFilename().c_str());

				std::string pathstr = imageItem->getPath();
				logger.log(CLogger::INFO, "Path of image: %s", pathstr.c_str());
				// This needs to be replaced with a factory class
				// ImageId object reads the image byte by byte for md5 and crc and Basic Exif.
				

				BasicExif_Ptr basicExifPtr = BasicExifFactory::make(pathstr);
				const BasicExif& basicExif = *basicExifPtr;
				basicExifPtr->debugPrint();
				// Add Image to the Index database if not a dup.
				logger.log(CLogger::INFO, "Find if dup %s", imageItem->getFilename().c_str());
				// If Make no changes == true, do not add this image into the dups index
				if (/*saCmdArgs.IsMakeNoChanges()*/1) { // DEBUG only allow dups
					// Force changes i.e the archive is damaged and images are being
					// added from a backup
					if (/*saCmdArgs.IsForceChanges()*/1) {
						if (m_imageIndex->IsDup(basicExif.getCrc())) {
							m_imageIndex->getData(basicExif.getCrc());
							logger.log(CLogger::INFO, "Dup %s", imageItem->getFilename().c_str());
							// reject image from import
							continue;
						}
					}
					else {
						// normal operation. dups are rejected 
						if (m_imageIndex->IsDup(basicExif.getCrc())) {
							//m_imageIndex->getData(imageId->getCrc());
							logger.log(CLogger::INFO, "Rejecting Dup from import %s", imageItem->getFilename().c_str());
							// reject image from import
							continue;
						}
						else {
							// Add To the Image Indexing (used to find duplicates)
							if (m_imageIndex->add(basicExif) == false) {
								logger.log(CLogger::INFO, "Dup %s", imageItem->getFilename().c_str());
								// reject image from import
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
				logger.log(CLogger::INFO, "Processing bulk metedata %s", imageItem->getFilename().c_str());
				MetadataTemplate metadataTemplate;
				std::string metatemplatePath = m_metatemplatePath;
				metatemplatePath += "/default.tpl";

				if (metadataTemplate.read(metatemplatePath.c_str()) == false) {
					logger.log(CLogger::INFO, "Cannot read default template file \"%s\"", metatemplatePath.c_str());
				}
				logger.log(CLogger::INFO, "Read metadata template file \"%s\"", metatemplatePath.c_str());
				// Creating metadata object with bulk data
				MetadataObject* metadataObject = metadataTemplate.getMetadataObject();
				if (exifObject != nullptr) {
					copyExif(metadataObject, exifObject);
				}
				imageGroup->add(basicExif, metadataObject);
				logger.log(CLogger::INFO, "completed step2 \"%s\"", imageItem->getFilename().c_str());
				/*
				XMLWriter xmlWriter;
				xmlWriter.writeImage(*metadataObject, "c:/temp/image.xml");
				*/
			}

		}
		for (std::vector<ImageGroup *>::iterator i = imageGroups.begin(); i != imageGroups.end(); i++) {
			ImageGroup *group = *i;
			for (std::vector<ImageContainer *>::iterator i = group->begin(); i != group->end(); i++) {
				ImageContainer *imageSet = *i;
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
				ExifDate date = m_archiveDate->getArchiveDate();
				imageSet->setTime(date.getTime());
				ImagePath imagePath(date.getTime());

				logger.log(CLogger::INFO, "Image: %s set to archive date: \"%s\"", imageSet->getName().c_str(), date.toString().c_str());
				if (imageSet->getComment().empty() == true) {
					imageSet->setComment("New Image");
				}

				std::string imageRootPath = imageSet->getPath();
				logger.log(CLogger::INFO, "Image(s) source path: \"%s\"", imageRootPath.c_str());
				imageSet->setImageRootPath(imagePath.getRelativePath());

				const BasicExifFactory *imageId;
				const MetadataObject *metadataObject;
				if (imageSet->hasPictureFile()) {
					const BasicExif &basicExif = imageSet->getPictureId();
					metadataObject = imageSet->getPictureMetadata();
					std::string picPath = imageSet->getPictureFile();
					imagePath.setImageName(picPath.c_str());

					if (CreateImage(basicExif, imagePath, csvDBFile, *((MetadataObject *)metadataObject)) == false) {
						return false;
					}
					
					// Create metadata xml file for this image.
					if (CreateMetadataXMLFile(imagePath, csvDBFile, *((MetadataObject *)metadataObject)) == false) {
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
					int seqNumber = csvDBFile.getMaxIndex();
					if (m_viewManager->add(seqNumber) == false) {
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
					metadataObject = imageSet->getRawMetadata();
					std::string rawPath = imageSet->getRawFile();
					imagePath.setImageName(rawPath.c_str());

					if (CreateImage(basicExif, imagePath, csvDBFile, *((MetadataObject *)metadataObject)) == false) {
						return false;
					}

					// Create metadata xml file for this image.
					if (CreateMetadataXMLFile(imagePath, csvDBFile, *((MetadataObject *)metadataObject)) == false) {
						return false;
					}
//					if (m_viewManager->add2Master(imagePath.getRelativePath().c_str(), imageSet->getRawFile()) == false) {
//						return false;
//					}

				}
				

			}
			if (!saCmdArgs.IsMakeNoChanges()) {
				// process ImageSet
				logger.log(CLogger::SUMMARY, "Starting Archiving phase");
				archiveRepository.Import(*group);
			}
		}

		//_CrtDumpMemoryLeaks();
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
	if (csvDBFile.insert(seqNumber, imagePath.getRelativePath().c_str(), imagePath.getImageName().c_str(), n, crc, basicExif.getMd5().c_str(),
																									basicExif.getUuid().c_str(), 0, date) == false) {
		return false;
	}
	return true;
}
// Create metadata xml file for this image.
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

bool ArchiveBuilder::processHistory(ImagePath &imagePath, const char *filepath, const char *comment, const HistoryEvent &he, int ver) {

	std::string fullPath = m_archivePath + "/" + filepath;
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
	sprintf(buff, "%.4d", ver);

	History &history = History::getHistory();
	imageHistory.add(filepath, buff, comment, he);
	history.add(filepath, buff, comment, he);

	return true;

}


// ? metadataObject->setAperture(exifObject-> std::string& aperture);
// ? metadataObject->setCompression(exifObject-> std::string& compression);
// ? metadataObject->setDepth(exifObject->getD std::string& depth);
// ? metadataObject->setDigitalZoom(exifObject-> std::string& digitalZoom);
// ? metadataObject->setExifVersion(exifObject-> std::string& exifVersion);
// ? metadataObject->setExposureProgram(exifObject-> std::string& exposureProgram);
// metadataObject->setGpsTimeStamp(exifObject-> gpsTimeStamp);
// ? metadataObject->setLightSource(exifObject-> std::string& lightSource);
// ? metadataObject->setPage(exifObject-> std::string& page);
// ? metadataObject->setPrimaryEncoding(exifObject-> std::string& primaryEncoding);
// ? metadataObject->setResolution(exifObject-> std::string& resolution);
// ? metadataObject->setSampleColor(exifObject-> std::string& sampleColor);
// ? metadataObject->setSensingMethod(exifObject-> std::string& sensingMethod);

void ArchiveBuilder::copyExif(MetadataObject* metadataObject, ExifObject *exifObject) {
	
	//metadataObject->setTitle(exifObject-> std::string& title); 
	//metadataObject->setSubject(exifObject-> std::string& subject);
	//metadataObject->setTags(exifObject-> std::string& tags);
	//metadataObject->setComments(exifObject-> std::string& comments);
	metadataObject->setAperture(exifObject->getFNumber());
	metadataObject->setCaptureDate(exifObject->getDateTimeOriginal());
	metadataObject->setColorSpace(exifObject->getColorSpace());
	metadataObject->setCompression(exifObject->getCompression());
	metadataObject->setCopyright(exifObject->getCopyright());
	//metadataObject->setDateAdded(exifObject-> std::string& dateAdded);
	//metadataObject->setDateCreate(exifObject-> std::string& dateCreate);
	//metadataObject->setDateModified(exifObject-> std::string& dateModified);
	metadataObject->setDepth(exifObject->getDepth());
	metadataObject->setDescription(exifObject->getImageDescription());
	metadataObject->setDigitalZoom(exifObject->getDigitalZoom());
	metadataObject->setExifVersion(exifObject->getExifVersion());
	metadataObject->setExposureBias(exifObject->getExposureBiasValue());
	metadataObject->setExposureProgram(exifObject->getExposureProgram());
	metadataObject->setExposureTime(exifObject->getExposureTime());
	metadataObject->setFlash(exifObject->getFlash());
	metadataObject->setFocalLength(exifObject->getFocalLength());
	// metadataObject->setGpsTimeStamp(exifObject-> gpsTimeStamp);
	metadataObject->setHeight(exifObject->getImageHeight());
	metadataObject->setIsoSpeedRating(exifObject->getIsoSpeedRatings());
	//metadataObject->setLabel(exifObject-> std::string& label);
	metadataObject->setLatitude(exifObject->getLatitude());
	// ? metadataObject->setLightSource(exifObject-> std::string& lightSource);
	metadataObject->setLongitude(exifObject->getLongitude());
	metadataObject->setMaker(exifObject->getMake());
	metadataObject->setMeteringMode(exifObject->getMeteringMode());
	metadataObject->setModel(exifObject->getModel());
	// ? metadataObject->setPage(exifObject-> std::string& page);
	// ? metadataObject->setPrimaryEncoding(exifObject-> std::string& primaryEncoding);
	//metadataObject->setRating(exifObject-> std::string& rating);
	// ? metadataObject->setResolution(exifObject-> std::string& resolution);
	// ? metadataObject->setSampleColor(exifObject-> std::string& sampleColor);
	// ? metadataObject->setSensingMethod(exifObject-> std::string& sensingMethod);
	//metadataObject->setSequenceId(exifObject-> std::string& sequenceId);
	metadataObject->setSoftware(exifObject->getSoftware());
	//metadataObject->setSourceUrl(exifObject-> std::string& sourceUrl);
	//metadataObject->setUsageRights(exifObject-> std::string& usageRights);
	metadataObject->setViewRotation(exifObject->getOrientation());
	metadataObject->setWidth(exifObject->getImageWidth());
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
