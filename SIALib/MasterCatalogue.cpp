#include "MasterCatalogue.h"
#include "XMLWriter.h"
#include "SAUtils.h"
#include "AppPaths.h"
#include "pathcontroller.h"
#include "HookCmd.h"
#include "ImagePageMaker.h"
#include "MenuPagesMaker.h"
#include "HTMLWriter.h"
#include "MetadataObject.h"
#include "ImageExtentions.h"
#include "CLogger.h"
#include <sstream>
#include <iostream>
#include <fstream>

namespace simplearchive {

	class HTMLMetadataWriter {
		static std::string writeTag(const char *tag, const std::string &value);
		bool writeFileInformation(MetadataObject &container, const char *path);
		bool writedescription(MetadataObject &container, const char *path);
		bool writeOrigin(MetadataObject &container, const char *path);
		bool writeImage(MetadataObject &container, const char *path);
		bool writeCamera(MetadataObject &container, const char *path);
		bool writeAdvancedPhoto(MetadataObject &container, const char *path);
		bool writeLocation(MetadataObject &container, const char *path);
		bool writeIPTC(MetadataObject &container, const char *path);
		bool writeSummary(MetadataObject &container, const char *path);
	public:
		HTMLMetadataWriter() {};
		virtual ~HTMLMetadataWriter() {};
		bool writeMetadata(MetadataObject &container, const char *path);
		//bool writeImage(MetadataObject &container, const char *path);
		//bool writeGroup(SideCar &sideCar, const char *path);
		//bool writeIndexImage(IndexMetadataObject &container, const char *path);
	};

	bool MasterCatalogue::settupSystemWWW(const char *archiveRootPath, const char *templatePath, const char* wwwCataloguePath, const char *systemHistoryPath, const char *inputJounalPath) {
		m_archiveRootPath = archiveRootPath;
		m_pageImageMaker->setArchiveRootPath(archiveRootPath);
		if (m_systemHistoryMaker->setPaths(templatePath, wwwCataloguePath, systemHistoryPath) == false) {
			return false;
		}
		if (m_inputJournalMaker->setPaths(templatePath, wwwCataloguePath, inputJounalPath) == false) {
			return false;
		}
		return true;
	}

	void MasterCatalogue::setWWWEnabled(bool b)
	{
		m_viewWWWPath->setEnabled(b);
	}

	void MasterCatalogue::setFileEnabled(bool b)
	{
		m_viewFilePath->setEnabled(b);
	}


	MasterCatalogue::MasterCatalogue() :
						m_viewFilePath(std::make_unique<ViewPath>()),
						m_viewWWWPath(std::make_unique<ViewPath>()),
						m_pageIndexMaker(std::make_unique<PageIndexMaker>()),
						m_pageImageMaker(std::make_unique<ImagePageMaker>()),
						m_systemHistoryMaker(std::make_unique<SystemHistoryMaker>()),
						m_inputJournalMaker(std::make_unique<ImportJournalMaker>())
	{
		m_fullSize = true;
		m_preview1 = true;
		m_preview2 = true;
		m_preview3 = true;
		m_thumbnail = true;

		m_fileFullSize = true;
		m_filePreview1 = true;
		m_filePreview2 = true;
		m_filePreview3 = true;
		m_fileThumbnail = true;

	}



	MasterCatalogue::~MasterCatalogue()
	{
	}

	bool MasterCatalogue::initWWW(const char *tempPath, const char *templatePath, const char* rootPath) {
		m_tempPath = tempPath;
		
		std::string imagesPath = rootPath;
		imagesPath += '/';
		imagesPath += "html";
		if (m_pageIndexMaker->setTemplatePath(templatePath, imagesPath.c_str()) == false) {
			return false;
		}
		
		if (m_pageImageMaker->setTemplatePath(templatePath, rootPath) == false) {
			return false;
		}

		

		return true;
	}

	

	bool MasterCatalogue::addImage(const char *sourcePath, MetadataObject &metadataObject, const char *imageRelativePath) {
		CLogger &logger = CLogger::getLogger();
		if (isFileEnabled()) {
			logger.log(LOG_OK, CLogger::Level::INFO, "Creating view files for image: %s", imageRelativePath);
			if (addFileImage(sourcePath, metadataObject, imageRelativePath) == false) {
				logger.log(LOG_UNABLE_CREATE_FILE_IMAGES, CLogger::Level::ERR, "Unable to creat view files for image: %s", sourcePath);
				return false;
			}
		}
		if (isWWWEnabled()) {
			logger.log(LOG_OK, CLogger::Level::INFO, "Creating Web files for image: %s", imageRelativePath);
			if (addWWWImage(sourcePath, metadataObject, imageRelativePath) == false) {
				logger.log(LOG_UNABLE_CREATE_WWW_IMAGES, CLogger::Level::ERR, "Unable to creat Web files for image: %s", sourcePath);
				return false;
			}
		}
		return true;
	}

	bool MasterCatalogue::addWWWImage(const char *sourcePath, MetadataObject &metadataObject, const char *imageRelativePath) {
		if (m_viewWWWPath->isEnabled()) {
			return true;
		}
		std::string yyyymmddimg = imageRelativePath;
		if (m_viewWWWPath->settupRelative(yyyymmddimg) == false) {
			return false;
		}

		addDayUpdate(m_viewWWWPath->getYearDay());

		std::string dist = m_viewWWWPath->getImagePath();
		std::string source = sourcePath;
		std::string file = m_viewWWWPath->getImageName();
		std::string ext = SAUtils::getExtention(file);
		ImageExtentions &ie = ImageExtentions::get();
		ExtentionItem &extItem = ie.find(ext.c_str());

		ImageType type = extItem.getType();
		std::string fileOut = file;
		if (type.getType() == ImageType::Type::PICTURE_EXT) {
			if (copyFile(source, m_tempPath, file) == false) {
				return false;
			}
			if (isWWWFullSize() == true) {
				if (copyFile(m_tempPath, dist, fileOut) == false) {
					return false;
				}
			}
			if (isWWWPreview1() == true || isWWWPreview2() == true || isWWWPreview3() == true || isWWWThumbnail() == true) {
				if (createWWWPreviews(m_tempPath, yyyymmddimg) == false) {
					return false;
				}
			}
		}
		else if (type.getType() == ImageType::Type::RAW_EXT) {
			fileOut += ".jpg";
			if (convertRAW(source, m_tempPath, file, fileOut) == false) {
				return false;
			}
			if (isWWWFullSize() == true) {
				if (copyFile(m_tempPath, dist, fileOut) == false) {
					return false;
				}
			}
			if (isWWWPreview1() == true || isWWWPreview2() == true || isWWWPreview3() == true || isWWWThumbnail() == true) {
				yyyymmddimg += ".jpg";
				if (createWWWPreviews(m_tempPath, yyyymmddimg) == false) {
					return false;
				}
			}
		}
		else {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_IMAGE_TYPE);
			return false;
		}
		

		

		std::string metaPath = m_viewWWWPath->getMetadataPath().c_str();
		if (createWWWMetadata(metadataObject, metaPath) == false) {
			return false;
		}
		
		return true;
	}

	bool MasterCatalogue::addFileImage(const char *sourcePath, MetadataObject &metadataObject, const char *imageRelativePath) {
		if (m_viewFilePath->isEnabled()) {
			return true;
		}
		std::string yyyymmddimg = imageRelativePath;
		if (m_viewFilePath->settupRelative(yyyymmddimg) == false) {
			return false;
		}


		std::string dist = m_viewFilePath->getImagePath();
		std::string source = sourcePath;
		std::string file = m_viewFilePath->getImageName();
		std::string ext = SAUtils::getExtention(file);
		ImageExtentions &ie = ImageExtentions::get();
		ExtentionItem &extItem = ie.find(ext.c_str());

		ImageType type = extItem.getType();
		std::string fileOut = file;
		if (type.getType() == ImageType::Type::PICTURE_EXT) {
			if (copyFile(source, m_tempPath, file) == false) {
				return false;
			}
			if (isFileFullSize() == true) {
				if (copyFile(m_tempPath, dist, fileOut) == false) {
					return false;
				}
			}
			if (isFilePreview1() == true || isFilePreview2() == true || isFilePreview3() == true || isFileThumbnail() == true) {
				if (createFilePreviews(m_tempPath, yyyymmddimg) == false) {
					return false;
				}
			}
		}
		else if (type.getType() == ImageType::Type::RAW_EXT) {
			fileOut += ".jpg";
			if (convertRAW(source, m_tempPath, file, fileOut) == false) {
				return false;
			}
			if (isFileFullSize() == true) {
				if (copyFile(m_tempPath, dist, fileOut) == false) {
					return false;
				}
			}
			if (isFilePreview1() == true || isFilePreview2() == true || isFilePreview3() == true || isFileThumbnail() == true) {
				yyyymmddimg += ".jpg";
				if (createFilePreviews(m_tempPath, yyyymmddimg) == false) {
					return false;
				}
			}
		}
		else {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_IMAGE_TYPE);
			return false;
		}

		std::string metaPath = m_viewFilePath->getMetadataPath().c_str();
		if (createFileMetadata(metadataObject, metaPath) == false) {
			return false;
		}
		return true;
	}


	void MasterCatalogue::addDayUpdate(std::string yyyymmddimg) {
		if (std::find(m_daysProcessed.begin(), m_daysProcessed.end(), yyyymmddimg) != m_daysProcessed.end()) {
			return;
		}
		m_daysProcessed.push_back(yyyymmddimg);
	}

	bool MasterCatalogue::makePages() {
		if (m_pageIndexMaker->process() == false) {
			return false;
		}
		if (m_pageImageMaker->process(m_daysProcessed) == false) {
			return false;
		}

		return true;
	}

	bool MasterCatalogue::processHistoryPages() {
		if (m_systemHistoryMaker->process() == false) {
			return false;
		}
		return true;
	}

	bool MasterCatalogue::processJournalPages() {
		if (m_inputJournalMaker->process() == false) {
			return false;
		}
		return true;
	}

	bool MasterCatalogue::processWWWPages()
	{
		if (isWWWEnabled()) {
			if (processHistoryPages() == false) {
				return false;
			}
			if (processJournalPages() == false) {
				return false;
			}
			if (makePages() == false) {
				return false;
			}
		}
		return true;
	}

	bool MasterCatalogue::createWWWMetadata(MetadataObject &metadataObject, std::string &imageRelativePath) {
		// Canon EOS 1D X | Canon EF 600mm f/4.0L IS II USM @840mm | 1/500 | f/5.6 | ISO 1000
		//metadataObject
		std::string metaPath = imageRelativePath;
		metaPath += '/';
		metaPath += m_viewWWWPath->getImageName();
		//metaPath += ".html";
		HTMLMetadataWriter htmlMetadataWriter;
		if (htmlMetadataWriter.writeMetadata(metadataObject, metaPath.c_str()) == false) {
			return false;
		}
		return true;
	}

	bool MasterCatalogue::createFileMetadata(MetadataObject &metadataObject, std::string &imageRelativePath) {
		
		//metadataObject
		std::string metaPath = imageRelativePath;
		metaPath += '/';
		metaPath += m_viewFilePath->getImageName();
		metaPath += ".xml";
		XMLWriter xmlWriter;
		if (xmlWriter.writeImage(metadataObject, metaPath.c_str()) == false) {
			return false;
		}
		return true;
	}

	bool MasterCatalogue::createWWWPreviews(std::string &sourcePath, std::string &yyyymmddStr) {

		if (m_viewWWWPath->settupRelative(yyyymmddStr) == false) {
			return false;
		}

		std::string dist = m_tempPath;
		std::string source = sourcePath;
		std::string file = m_viewWWWPath->getImageName();

		
		std::string name = file.substr(0, file.find_last_of("."));
		std::string ext = file.substr(file.find_last_of(".") + 1);



		if (isWWWPreview1() == true) {
			std::string preview1Name = name;
			preview1Name += "_1.";
			preview1Name += ext;

			std::string tmpFolderSource = m_tempPath;
			tmpFolderSource += '/';
			tmpFolderSource += m_viewWWWPath->getImageName();

			std::string tmpFolderDist = m_tempPath;
			tmpFolderDist += '/';
			tmpFolderDist += preview1Name;
			OnViewPreview1Cmd onViewPreview1Cmd(tmpFolderSource.c_str(), tmpFolderDist.c_str());
			onViewPreview1Cmd.process();

			if (copyFile(m_tempPath, m_viewWWWPath->getImagePath(), preview1Name, preview1Name) == false) {
				return false;
			}
			std::string tmpFile = m_tempPath;
			tmpFile += '/'; tmpFile += preview1Name;
			if (!SAUtils::delFile(tmpFile.c_str())) {
				return false;
			}
		}
		if (isWWWPreview3() == true) {
			std::string preview3Name = name;
			preview3Name += "_3.";
			preview3Name += ext;

			std::string tmpFolderSource = m_tempPath;
			tmpFolderSource += '/';
			tmpFolderSource += m_viewWWWPath->getImageName();

			std::string tmpFolderDist = m_tempPath;
			tmpFolderDist += '/';
			tmpFolderDist += preview3Name;
			OnViewPreview3Cmd onViewPreview3Cmd(tmpFolderSource.c_str(), tmpFolderDist.c_str());
			onViewPreview3Cmd.process();

			if (copyFile(m_tempPath, m_viewWWWPath->getImagePath(), preview3Name, preview3Name) == false) {
				return false;
			}
			std::string tmpFile = m_tempPath;
			tmpFile += '/'; tmpFile += preview3Name;
			if (!SAUtils::delFile(tmpFile.c_str())) {
				return false;
			}
		}
		if (isWWWPreview2() == true) {
			std::string preview2Name = name;
			preview2Name += "_2.";
			preview2Name += ext;

			std::string tmpFolderSource = m_tempPath;
			tmpFolderSource += '/';
			tmpFolderSource += m_viewWWWPath->getImageName();

			std::string tmpFolderDist = m_tempPath;
			tmpFolderDist += '/';
			tmpFolderDist += preview2Name;
			OnViewPreview2Cmd onViewPreview2Cmd(tmpFolderSource.c_str(), tmpFolderDist.c_str());
			onViewPreview2Cmd.process();

			if (copyFile(m_tempPath, m_viewWWWPath->getImagePath(), preview2Name, preview2Name) == false) {
				return false;
			}
			std::string tmpFile = m_tempPath;
			tmpFile += '/'; tmpFile += preview2Name;
			if (!SAUtils::delFile(tmpFile.c_str())) {
				return false;
			}
		}
		if (isWWWThumbnail() == true) {
			std::string thumbnailName = name;
			thumbnailName += "_t.";
			thumbnailName += ext;

			std::string tmpFolderSource = m_tempPath;
			tmpFolderSource += '/';
			tmpFolderSource += m_viewWWWPath->getImageName();

			std::string tmpFolderDist = m_tempPath;
			tmpFolderDist += '/';
			tmpFolderDist += thumbnailName;
			OnViewThumbnailCmd onViewThumbnailCmd(tmpFolderSource.c_str(), tmpFolderDist.c_str());
			onViewThumbnailCmd.process();

			if (copyFile(m_tempPath, m_viewWWWPath->getImagePath(), thumbnailName, thumbnailName) == false) {
				return false;
			}
			std::string tmpFile = m_tempPath;
			tmpFile += '/'; tmpFile += thumbnailName;
			if (!SAUtils::delFile(tmpFile.c_str())) {
				return false;
			}
		}
		std::string tempFile = m_tempPath;
		tempFile += '/';
		tempFile += m_viewWWWPath->getImageName();
		if (SAUtils::delFile(tempFile.c_str())) {

		}
		return true;
	}

	bool MasterCatalogue::createFilePreviews(std::string &sourcePath, std::string &yyyymmddStr) {

		if (m_viewFilePath->settupRelative(yyyymmddStr) == false) {
			return false;
		}

		std::string dist = m_tempPath;
		std::string source = sourcePath;
		std::string file = m_viewFilePath->getImageName();


		std::string name = file.substr(0, file.find_last_of("."));
		std::string ext = file.substr(file.find_last_of(".") + 1);



		if (isFilePreview1() == true) {
			std::string preview1Name = name;
			preview1Name += "_1.";
			preview1Name += ext;

			std::string tmpFolderSource = m_tempPath;
			tmpFolderSource += '/';
			tmpFolderSource += m_viewFilePath->getImageName();

			std::string tmpFolderDist = m_tempPath;
			tmpFolderDist += '/';
			tmpFolderDist += preview1Name;
			OnViewPreview1Cmd onViewPreview1Cmd(tmpFolderSource.c_str(), tmpFolderDist.c_str());
			onViewPreview1Cmd.process();

			if (copyFile(m_tempPath, m_viewFilePath->getImagePath(), preview1Name, preview1Name) == false) {
				return false;
			}
			std::string tmpFile = m_tempPath;
			tmpFile += '/'; tmpFile += preview1Name;
			if (!SAUtils::delFile(tmpFile.c_str())) {
				return false;
			}
		}
		if (isFilePreview3() == true) {
			std::string preview3Name = name;
			preview3Name += "_3.";
			preview3Name += ext;

			std::string tmpFolderSource = m_tempPath;
			tmpFolderSource += '/';
			tmpFolderSource += m_viewFilePath->getImageName();

			std::string tmpFolderDist = m_tempPath;
			tmpFolderDist += '/';
			tmpFolderDist += preview3Name;
			OnViewPreview3Cmd onViewPreview3Cmd(tmpFolderSource.c_str(), tmpFolderDist.c_str());
			onViewPreview3Cmd.process();

			if (copyFile(m_tempPath, m_viewFilePath->getImagePath(), preview3Name, preview3Name) == false) {
				return false;
			}
			std::string tmpFile = m_tempPath;
			tmpFile += '/'; tmpFile += preview3Name;
			if (!SAUtils::delFile(tmpFile.c_str())) {
				return false;
			}
		}
		if (isFilePreview2() == true) {
			std::string preview2Name = name;
			preview2Name += "_2.";
			preview2Name += ext;

			std::string tmpFolderSource = m_tempPath;
			tmpFolderSource += '/';
			tmpFolderSource += m_viewFilePath->getImageName();

			std::string tmpFolderDist = m_tempPath;
			tmpFolderDist += '/';
			tmpFolderDist += preview2Name;
			OnViewPreview2Cmd onViewPreview2Cmd(tmpFolderSource.c_str(), tmpFolderDist.c_str());
			onViewPreview2Cmd.process();

			if (copyFile(m_tempPath, m_viewFilePath->getImagePath(), preview2Name, preview2Name) == false) {
				return false;
			}
			std::string tmpFile = m_tempPath;
			tmpFile += '/'; tmpFile += preview2Name;
			if (!SAUtils::delFile(tmpFile.c_str())) {
				return false;
			}
		}
		if (isFileThumbnail() == true) {
			std::string thumbnailName = name;
			thumbnailName += "_t.";
			thumbnailName += ext;

			std::string tmpFolderSource = m_tempPath;
			tmpFolderSource += '/';
			tmpFolderSource += m_viewFilePath->getImageName();

			std::string tmpFolderDist = m_tempPath;
			tmpFolderDist += '/';
			tmpFolderDist += thumbnailName;
			OnViewThumbnailCmd onViewThumbnailCmd(tmpFolderSource.c_str(), tmpFolderDist.c_str());
			onViewThumbnailCmd.process();

			if (copyFile(m_tempPath, m_viewFilePath->getImagePath(), thumbnailName, thumbnailName) == false) {
				return false;
			}
			std::string tmpFile = m_tempPath;
			tmpFile += '/'; tmpFile += thumbnailName;
			if (!SAUtils::delFile(tmpFile.c_str())) {
				return false;
			}
		}
		std::string tempFile = m_tempPath;
		tempFile += '/';
		tempFile += m_viewFilePath->getImageName();
		if (SAUtils::delFile(tempFile.c_str())) {

		}
		return true;
	}

	bool MasterCatalogue::copyFile(const std::string &source, const std::string &dist, const std::string &file) {

		std::string from = source + "/" + file;
		std::string to = dist + '/' + file;

		if (SAUtils::FileExists(from.c_str()) == false) {
			return false;
		}
		if (SAUtils::DirExists(dist.c_str()) == false) {
			return false;
		}
		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			return false;
		}

		return true;
	}

	bool MasterCatalogue::convertRAW(const std::string &source, const std::string &dist, const std::string &fileIn, const std::string &fileOut) {

		std::string from = source + "/" + fileIn;
		std::string to = dist + '/' + fileOut;

		if (SAUtils::FileExists(from.c_str()) == false) {
			return false;
		}
		if (SAUtils::DirExists(dist.c_str()) == false) {
			return false;
		}

		OnViewRAWCmd onViewRAWCmd(from.c_str(), to.c_str());
		onViewRAWCmd.process();

		return true;
	}

	bool MasterCatalogue::copyFile(const std::string &source, const std::string &dist, const std::string &sfile, const std::string &dfile) {

		std::string from = source + "/" + sfile;
		std::string to = dist + '/' + dfile;

		if (SAUtils::FileExists(from.c_str()) == false) {
			return false;
		}
		if (SAUtils::DirExists(dist.c_str()) == false) {
			return false;
		}
		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			return false;
		}

		return true;
	}

	void MasterCatalogue::setWWWFullSize(bool b) {
		m_fullSize = b;
	}
	void MasterCatalogue::setWWWPreview1(bool b) {

		m_preview1 = b;
	}
	void MasterCatalogue::setWWWThumbnail(bool b) {

		m_thumbnail = b;
	}

	bool MasterCatalogue::isWWWFullSize() {
		return m_fullSize;
	}
	bool MasterCatalogue::isWWWPreview1() {
		return m_preview1;
	}
	bool MasterCatalogue::isWWWPreview2() {
		return m_preview2;
	}
	bool MasterCatalogue::isWWWPreview3() {
		return m_preview3;
	}
	
	bool MasterCatalogue::isWWWThumbnail() {
		return m_thumbnail;
	}

	bool MasterCatalogue::isFileFullSize() {
		return m_fileFullSize;
	}
	bool MasterCatalogue::isFilePreview1() {
		return m_filePreview1;
	}
	bool MasterCatalogue::isFilePreview2() {
		return m_filePreview2;
	}
	bool MasterCatalogue::isFilePreview3() {
		return m_filePreview3;
	}

	bool MasterCatalogue::isFileThumbnail() {
		return m_thumbnail;
	}

	bool ViewPath::settup() {


		// Master Archive
		std::string dataFolder = m_pathToRepository;
		if (SAUtils::DirExists(dataFolder.c_str()) == false) {
			if (SAUtils::mkDir(dataFolder.c_str()) == false) {
				return false;
			}
		}


		if (m_indexPath.empty() == true) {

			m_indexPath = dataFolder + IMAGES_PATH;

			if (SAUtils::DirExists(m_indexPath.c_str()) == false) {
				if (SAUtils::mkDir(m_indexPath.c_str()) == false) {
					return false;
				}
			}

		}
		return true;
	}



	bool ViewPath::settupRelative(std::string &yyyymmddStr) {

		PathController pathController;

		pathController.splitPathAndFile(yyyymmddStr.c_str());
		m_imageName = pathController.getImageName();
		m_yearStr = yyyymmddStr.substr(0, 4);
		m_yyyymmddStr = pathController.getYearday();

		m_relpath = pathController.getRelativePath();


		m_yearStrPath = m_indexPath + '/' + pathController.getYear();
		if (SAUtils::DirExists(m_yearStrPath.c_str()) == false) {
			if (SAUtils::mkDir(m_yearStrPath.c_str()) == false) {
				return false;
			}

		}

		m_yyyymmddStrPath = m_yearStrPath + '/' + pathController.getYearday();
		if (SAUtils::DirExists(m_yyyymmddStrPath.c_str()) == false) {
			if (SAUtils::mkDir(m_yyyymmddStrPath.c_str()) == false) {
				return false;
			}

		}
		m_imagePath = m_yyyymmddStrPath + MASTER_IMAGE_PATH;
		m_metadataPath = m_yyyymmddStrPath + METADATA_PATH;


		if (SAUtils::DirExists(m_imagePath.c_str()) == false) {
			if (SAUtils::mkDir(m_imagePath.c_str()) == false) {
				return false;
			}

		}
		if (SAUtils::DirExists(m_metadataPath.c_str()) == false) {
			if (SAUtils::mkDir(m_metadataPath.c_str()) == false) {
				return false;
			}

		}



		//IntegrityManager &integrityManager = IntegrityManager::get();
		//integrityManager.addDayFolder(m_yyyymmddStr.c_str());
		return true;
	}





	bool ViewPath::isEnabled() {
		return m_enabled;
	}

	void ViewPath::setImageName(const char *imageName) {
		m_imageName = imageName;
	}

	const std::string& ViewPath::getImageName() const {
		return m_imageName;
	}

	const std::string& ViewPath::getRelativePath() const {
		return m_relpath;
	}



	std::string ViewPath::getMetadataPath() const {
		return m_metadataPath;
	}

	void ViewPath::setMetadataPath(std::string& metadataPath) {
		m_metadataPath = metadataPath;
	}

	std::string ViewPath::getImagePath() {
		return m_imagePath;
	}

	void ViewPath::setEnabled(bool b) {
		m_enabled = b;
	}

	void ViewPath::setRepositoryPath(const char *pathToRepository) {
		m_pathToRepository = pathToRepository;
	}

	std::string  ViewPath::getRepositoryPath() const {
		return m_pathToRepository;
	}

	std::string ViewPath::getYearDay() {
		return m_yyyymmddStr;
	}

	bool HTMLMetadataWriter::writeMetadata(MetadataObject &container, const char *path) {
		
		std::string infoPath = path;
		infoPath += ".fileinfo.html";
		if (writeFileInformation(container, infoPath.c_str()) == false) {
			return false;
		}

		std::string descriptionPath = path;
		descriptionPath += ".description.html";
		if (writedescription(container, descriptionPath.c_str()) == false) {
			return false;
		}

		std::string originPath = path;
		originPath += ".origin.html";
		if (writeOrigin(container, originPath.c_str()) == false) {
			return false;
		}

		std::string imagePath = path;
		imagePath += ".image.html";
		if (writeImage(container, imagePath.c_str()) == false) {
			return false;
		}

		std::string cameraPath = path;
		cameraPath += ".camera.html";
		if (writeCamera(container, cameraPath.c_str()) == false) {
			return false;
		}

		std::string photoPath = path;
		photoPath += ".photo.html";
		if (writeAdvancedPhoto(container, photoPath.c_str()) == false) {
			return false;
		}

		std::string locPath = path;
		locPath += ".location.html";
		if (writeLocation(container, locPath.c_str()) == false) {
			return false;
		}

		std::string iptcPath = path;
		iptcPath += ".iptc.html";
		if (writeIPTC(container, iptcPath.c_str()) == false) {
			return false;
		}

		std::string summaryPath = path;
		summaryPath += ".summary.int";
		if (writeSummary(container, summaryPath.c_str()) == false) {
			return false;
		}
		return true;
	}
	/*
	bool HTMLMetadataWriter::writeImage(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"file_block\">\n"
			<< "<table class=\"image_attr\">\n"
			<< "<Caption>File</Caption>\n"
			<< writeTag(DB_SEQUENCEID, container.getSequenceIdString())	// *
			<< writeTag(DB_FILENAME, container.getFilenameString())
			<< writeTag(DB_ORGINALNAME, container.getOrginalNameString())
			<< writeTag(DB_FILEPATH, container.getFilepathString())
			<< writeTag(DB_UUID, container.getUniqueIdString())
			<< writeTag(DB_MEDIATYPE, container.getMediaTypeString())
			<< writeTag(DB_MD5, container.getMd5String())
			<< writeTag(DB_CRC, container.getCrcString())
			<< writeTag(DB_FILESIZE, container.getFileSizeString())
			<< writeTag(DB_DATECREATE, container.getDateCreateString())
			<< writeTag(DB_DATEMODIFIED, container.getDateModifiedString())
			<< writeTag(DB_DATEADDED, container.getAddedDateString())
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"description_block\">\n"
			<< "<table class=\"image_attr\">\n" // Description
			<< "<Caption>Description</Caption>\n"
			<< writeTag(DB_DESCRIPTION, container.getDescriptionString())
			<< writeTag(DB_LABEL, container.getLabelString())
			<< writeTag(DB_TITLE, container.getTitleString())
			<< writeTag(DB_SUBJECT, container.getSubjectString())
			<< writeTag(DB_RATING, container.getRatingString())
			<< writeTag(DB_RATINGPERCENT, container.getRatingPercentString())
			<< writeTag(DB_KEYWORDS, container.getKeywordsString()) // template
			<< writeTag(DB_VERSION, container.getVersionString()) // template
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"origin_block\">\n"
			<< "<table class=\"image_attr\">\n" // Origin
			<< "<Caption>Origin</Caption>\n"
			<< writeTag(DB_COMMENT, container.getCommentString())// template
			<< writeTag(DB_AUTHOR, container.getAuthorString())
			<< writeTag(DB_CAPTUREDATE, container.getCaptureDateString())
			<< writeTag(DB_SUBSETIMEORIGINAL, container.getSubjectDIstanceRangeString())
			<< writeTag(DB_COPYRIGHT, container.getCopyrightString())
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"image_block\">\n"
			<< "<table class=\"image_attr\">\n" // Image
			<< "<Caption>Image</Caption>\n"
			<< writeTag(DB_WIDTH, container.getWidthString())
			<< writeTag(DB_HEIGHT, container.getHeightString())
			<< writeTag(DB_VIEWROTATION, container.getViewRotationString())
			<< writeTag(DB_BITSPERSAMPLE, container.getBitsPerSampleString())
			<< writeTag(DB_XRESOLUTION, container.getXResolutionString())
			<< writeTag(DB_YRESOLUTION, container.getYResolutionString())
			<< writeTag(DB_RESOLUTIONUNIT, container.getResolutionUnitString())
			<< writeTag(DB_COMPRESSION, container.getCompressionString())
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"camera_block\">\n"
			<< "<table class=\"image_attr\">\n" // Camera
			<< "<Caption>Camera</Caption>\n"
			<< writeTag(DB_MAKER, container.getMakerString())
			<< writeTag(DB_MODEL, container.getModelString())
			<< writeTag(DB_EXPOSURETIME, container.getExposureTimeString())
			<< writeTag(DB_APERTURE, container.getApertureString())
			<< writeTag(DB_ISOSPEEDRATING, container.getIsoSpeedRatingString())
			<< writeTag(DB_EXPOSUREBIAS, container.getExposureBiasString())
			<< writeTag(DB_EXPOSUREPROGRAM, container.getExposureProgramString())
			<< writeTag(DB_SUBJECTDISTANCE, container.getSubjectDistanceString())
			<< writeTag(DB_FOCALLENGTH, container.getFocalLengthString())
			<< writeTag(DB_FOCALLENGTHIN35MM, container.getFocalLengthIn35mmString())
			<< writeTag(DB_FLASH, container.getFlashString())
			<< writeTag(DB_METERINGMODE, container.getMeteringModeString())
			<< writeTag(DB_DIGITALZOOM, container.getDigitalZoomString())
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"advanced_photo_block\">\n"
			<< "<table class=\"image_attr\">\n" // Advanced photo
			<< "<Caption>Advanced photo</Caption>\n"
			<< writeTag(DB_LIGHTSOURCE, container.getLightSourceString())
			<< writeTag(DB_COLORSPACE, container.getColorSpaceString())
			<< writeTag(DB_WHITEBALANCE, container.getWhiteBalanceString())
			<< writeTag(DB_SCENECAPTURETYPE, container.getSceneCaptureTypeString())
			<< writeTag(DB_CONTRAST, container.getContrastString())
			<< writeTag(DB_SATURATION, container.getSaturationString())
			<< writeTag(DB_SHARPNESS, container.getSharpnessString())
			<< writeTag(DB_BRIGHTNESSVALUE, container.getBrightnessValueString())
			<< writeTag(DB_SUBJECTDISTANCERANGE, container.getSubjectDIstanceRangeString())
			<< writeTag(DB_MAXAPERTURE, container.getMaxApertureString())
			<< writeTag(DB_COMPRESSEDBITSPERPIXEL, container.getCompressedBitsPerPixelString())
			<< writeTag(DB_FLASHENERGY, container.getFlashEnergyString())
			<< writeTag(DB_EXIFVERSION, container.getExifVersionString())
			<< writeTag(DB_SOFTWARE, container.getSoftwareString())
			<< writeTag(DB_PHOTOMETRICINTERPRETATION, container.getPhotometricInterpretationString())
			<< writeTag(DB_GPSTIMESTAMP, container.getGpsTimeStampString())
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"location_block\">\n"
			<< "<table class=\"image_attr\">\n"
			// Location
			<< "<Caption>Location</Caption>\n"
			<< writeTag(DB_LATITUDE, container.getLatitudeString())
			<< writeTag(DB_LONGITUDE, container.getLongitudeString())
			<< writeTag(DB_LOCATION, container.getLoctionString())
			<< writeTag(DB_SCENE, container.getSceneString())// template
			<< "</table>\n"
			<< "</div>\n"
			<< "<div class=\"iptc_block\">\n"
			<< "<table class=\"image_attr\">\n"			// IPTC
			<< "<Caption>IPTC</Caption>\n"
			<< writeTag(DB_SOURCEURL, container.getSourceUrlString())		// template
			<< writeTag(DB_USAGERIGHTS, container.getUsageRightsString())	// template
			<< writeTag(DB_COPYRIGHTURL, container.getCopyrightString())    // template
			<< writeTag(DB_HEADLINE, container.getHeadlineString())         // template
			<< writeTag(DB_CATEGORY, container.getCategoryString())			// template
			<< writeTag(DB_SOURCE, container.getSourceString())             // template
			<< writeTag(DB_INSTRUCTIONS, container.getInstructionsString()) // template
			<< writeTag(DB_CREATOR, container.getCreatorString())           // template
			<< writeTag(DB_JOBTITLE, container.getJobTitleString())         // template
			<< writeTag(DB_ADDRESS, container.getAddressString())			// template
			<< writeTag(DB_CITY, container.getCityString())					// template
			<< writeTag(DB_STATE, container.getStateString())				// template
			<< writeTag(DB_POSTALCODE, container.getPostalCodeString())		// template
			<< writeTag(DB_COUNTRY, container.getCountryString())			// template
			<< writeTag(DB_PHONE, container.getPhoneString())				// template
			<< writeTag(DB_EMAIL, container.getEmailString())				// template
			<< writeTag(DB_WEBSITE, container.getWebsiteString())			// template
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();

		return true;

	}
	*/

	bool HTMLMetadataWriter::writeSummary(MetadataObject &container, const char *path) {

		std::ofstream intFile;
		intFile.open(path);
		intFile << "Canon EOS 1D X | Canon EF 600mm f/4.0L IS II USM @840mm | 1/500 | f/5.6 | ISO 1000";
		intFile.close();
		return true;

	}

	bool HTMLMetadataWriter::writeFileInformation(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"file_block\">\n"
			<< "<table class=\"image_attr\">\n"
			<< "<Caption>File Information</Caption>\n"
			<< writeTag(DB_SEQUENCEID, container.getSequenceIdString())	// *
			<< writeTag(DB_FILENAME, container.getFilenameString())
			<< writeTag(DB_ORGINALNAME, container.getOrginalNameString())
			<< writeTag(DB_FILEPATH, container.getFilepathString())
			<< writeTag(DB_UUID, container.getUniqueIdString())
			<< writeTag(DB_MEDIATYPE, container.getMediaTypeString())
			<< writeTag(DB_MD5, container.getMd5String())
			<< writeTag(DB_CRC, container.getCrcString())
			<< writeTag(DB_FILESIZE, container.getFileSizeString())
			<< writeTag(DB_DATECREATE, container.getDateCreateString())
			<< writeTag(DB_DATEMODIFIED, container.getDateModifiedString())
			<< writeTag(DB_DATEADDED, container.getAddedDateString())
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();
		return true;

	}

	bool HTMLMetadataWriter::writedescription(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "</div>\n"
			<< "<div class=\"description_block\">\n"
			<< "<table class=\"image_attr\">\n" // Description
			<< "<Caption>Description</Caption>\n"
			<< writeTag(DB_DESCRIPTION, container.getDescriptionString())
			<< writeTag(DB_LABEL, container.getLabelString())
			<< writeTag(DB_TITLE, container.getTitleString())
			<< writeTag(DB_SUBJECT, container.getSubjectString())
			<< writeTag(DB_RATING, container.getRatingString())
			<< writeTag(DB_RATINGPERCENT, container.getRatingPercentString())
			<< writeTag(DB_KEYWORDS, container.getKeywordsString()) // template
			<< writeTag(DB_VERSION, container.getVersionString()) // template
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();
		return true;

	}

	bool HTMLMetadataWriter::writeOrigin(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"origin_block\">\n"
			<< "<table class=\"image_attr\">\n" // Origin
			<< "<Caption>Origin</Caption>\n"
			<< writeTag(DB_COMMENT, container.getCommentString())// template
			<< writeTag(DB_AUTHOR, container.getAuthorString())
			<< writeTag(DB_CAPTUREDATE, container.getCaptureDateString())
			<< writeTag(DB_SUBSETIMEORIGINAL, container.getSubjectDIstanceRangeString())
			<< writeTag(DB_COPYRIGHT, container.getCopyrightString())
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();
		return true;

	}

	

	bool HTMLMetadataWriter::writeImage(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"image_block\">\n"
			<< "<table class=\"image_attr\">\n" // Image
			<< "<Caption>Image</Caption>\n"
			<< writeTag(DB_WIDTH, container.getWidthString())
			<< writeTag(DB_HEIGHT, container.getHeightString())
			<< writeTag(DB_VIEWROTATION, container.getViewRotationString())
			<< writeTag(DB_BITSPERSAMPLE, container.getBitsPerSampleString())
			<< writeTag(DB_XRESOLUTION, container.getXResolutionString())
			<< writeTag(DB_YRESOLUTION, container.getYResolutionString())
			<< writeTag(DB_RESOLUTIONUNIT, container.getResolutionUnitString())
			<< writeTag(DB_COMPRESSION, container.getCompressionString())
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();
		return true;

	}

	bool HTMLMetadataWriter::writeCamera(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"camera_block\">\n"
			<< "<table class=\"image_attr\">\n" // Camera
			<< "<Caption>Camera</Caption>\n"
			<< writeTag(DB_MAKER, container.getMakerString())
			<< writeTag(DB_MODEL, container.getModelString())
			<< writeTag(DB_EXPOSURETIME, container.getExposureTimeString())
			<< writeTag(DB_APERTURE, container.getApertureString())
			<< writeTag(DB_ISOSPEEDRATING, container.getIsoSpeedRatingString())
			<< writeTag(DB_EXPOSUREBIAS, container.getExposureBiasString())
			<< writeTag(DB_EXPOSUREPROGRAM, container.getExposureProgramString())
			<< writeTag(DB_SUBJECTDISTANCE, container.getSubjectDistanceString())
			<< writeTag(DB_FOCALLENGTH, container.getFocalLengthString())
			<< writeTag(DB_FOCALLENGTHIN35MM, container.getFocalLengthIn35mmString())
			<< writeTag(DB_FLASH, container.getFlashString())
			<< writeTag(DB_METERINGMODE, container.getMeteringModeString())
			<< writeTag(DB_DIGITALZOOM, container.getDigitalZoomString())
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();
		return true;

	}


	bool HTMLMetadataWriter::writeAdvancedPhoto(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"advanced_photo_block\">\n"
			<< "<table class=\"image_attr\">\n" // Advanced photo
			<< "<Caption>Advanced photo</Caption>\n"
			<< writeTag(DB_LIGHTSOURCE, container.getLightSourceString())
			<< writeTag(DB_COLORSPACE, container.getColorSpaceString())
			<< writeTag(DB_WHITEBALANCE, container.getWhiteBalanceString())
			<< writeTag(DB_SCENECAPTURETYPE, container.getSceneCaptureTypeString())
			<< writeTag(DB_CONTRAST, container.getContrastString())
			<< writeTag(DB_SATURATION, container.getSaturationString())
			<< writeTag(DB_SHARPNESS, container.getSharpnessString())
			<< writeTag(DB_BRIGHTNESSVALUE, container.getBrightnessValueString())
			<< writeTag(DB_SUBJECTDISTANCERANGE, container.getSubjectDIstanceRangeString())
			<< writeTag(DB_MAXAPERTURE, container.getMaxApertureString())
			<< writeTag(DB_COMPRESSEDBITSPERPIXEL, container.getCompressedBitsPerPixelString())
			<< writeTag(DB_FLASHENERGY, container.getFlashEnergyString())
			<< writeTag(DB_EXIFVERSION, container.getExifVersionString())
			<< writeTag(DB_SOFTWARE, container.getSoftwareString())
			<< writeTag(DB_PHOTOMETRICINTERPRETATION, container.getPhotometricInterpretationString())
			<< writeTag(DB_GPSTIMESTAMP, container.getGpsTimeStampString())
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();
		return true;

	}

	bool HTMLMetadataWriter::writeLocation(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"location_block\">\n"
			<< "<table class=\"image_attr\">\n"
			// Location
			<< "<Caption>Location</Caption>\n"
			<< writeTag(DB_LATITUDE, container.getLatitudeString())
			<< writeTag(DB_LONGITUDE, container.getLongitudeString())
			<< writeTag(DB_LOCATION, container.getLoctionString())
			<< writeTag(DB_SCENE, container.getSceneString())// template
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();
		return true;

	}

	bool HTMLMetadataWriter::writeIPTC(MetadataObject &container, const char *path) {

		std::ofstream htmlFile;
		htmlFile.open(path);
		htmlFile << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"iptc_block\">\n"
			<< "<table class=\"image_attr\">\n"			// IPTC
			<< "<Caption>IPTC</Caption>\n"
			<< writeTag(DB_SOURCEURL, container.getSourceUrlString())		// template
			<< writeTag(DB_USAGERIGHTS, container.getUsageRightsString())	// template
			<< writeTag(DB_COPYRIGHTURL, container.getCopyrightString())    // template
			<< writeTag(DB_HEADLINE, container.getHeadlineString())         // template
			<< writeTag(DB_CATEGORY, container.getCategoryString())			// template
			<< writeTag(DB_SOURCE, container.getSourceString())             // template
			<< writeTag(DB_INSTRUCTIONS, container.getInstructionsString()) // template
			<< writeTag(DB_CREATOR, container.getCreatorString())           // template
			<< writeTag(DB_JOBTITLE, container.getJobTitleString())         // template
			<< writeTag(DB_ADDRESS, container.getAddressString())			// template
			<< writeTag(DB_CITY, container.getCityString())					// template
			<< writeTag(DB_STATE, container.getStateString())				// template
			<< writeTag(DB_POSTALCODE, container.getPostalCodeString())		// template
			<< writeTag(DB_COUNTRY, container.getCountryString())			// template
			<< writeTag(DB_PHONE, container.getPhoneString())				// template
			<< writeTag(DB_EMAIL, container.getEmailString())				// template
			<< writeTag(DB_WEBSITE, container.getWebsiteString())			// template
			<< "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>";
		htmlFile.close();
		return true;

	}

	std::string HTMLMetadataWriter::writeTag(const char *tag, const std::string &value) {
		std::ostringstream html;
		if (value.length() != 0) {
			html << "<tr> <td>" << tag << "</td> <td>" << value << "</td></tr>\n";
		}
		return html.str();
	}
}