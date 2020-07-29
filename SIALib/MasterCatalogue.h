#pragma once
#include <string>
#include <memory>
#include <vector>
#include "PageMakerBase.h"

namespace simplearchive {
	using namespace std;

	class MetadataObject;
	class PageIndexMaker;
	class ImagePageMaker;
	class SystemHistoryMaker;
	class ImportJournalMaker;
	
	class ViewPath {

		std::string m_relpath;
		std::string m_imagePath;
		std::string m_imageName;
		std::string m_yearStr;
		std::string m_yyyymmddStr;
		std::string m_yearStrPath;				//< i.e. C:\ProgramData\IDK Software\ImageArchive1.0\Master\2015
		std::string m_yyyymmddStrPath;			//< i.e. C:\ProgramData\IDK Software\ImageArchive1.0\Master\2015\2015-11-26
		std::string m_pathToRepository;
		std::string m_metadataPath;
		std::string m_indexPath;
		
		bool m_enabled;
	public:
		ViewPath() = default;
		virtual ~ViewPath() = default;
		bool settup();
		bool settupRelative(std::string &yyyymmddStr);
		bool isEnabled();
		void setEnabled(bool b);
		void setRepositoryPath(const char *pathToRepository);

		void setImageName(const char *imageName);

		const std::string& getImageName() const;
		const std::string& getRelativePath() const;

		std::string getRepositoryPath() const;
		std::string getImagePath();
		std::string getYearDay();
		std::string getMetadataPath() const;
		void setMetadataPath(std::string &metadataPath);
		

	};



	class MasterCatalogue {
		std::unique_ptr<ViewPath> m_viewFilePath;
		
		bool m_FileEnabled;
		bool m_fullSize;
		bool m_preview1;
		bool m_preview2;
		bool m_preview3;
		bool m_thumbnail;

		bool m_fileFullSize;
		bool m_filePreview1;
		bool m_filePreview2;
		bool m_filePreview3;
		bool m_fileThumbnail;

		std::string m_tempPath;
		std::string m_archiveRootPath;

		bool createFilePreviews(std::string &source, std::string &imageRelativePath);
		bool createFileMetadata(MetadataObject &metadataObject, std::string &imageRelativePath);
		bool copyFile(const std::string &source, const std::string &dist, const std::string &file);
		bool copyFile(const std::string &source, const std::string &dist, const std::string &sfile, const std::string &dfile);
		bool convertRAW(const std::string &source, const std::string &dist, const std::string &fileOut);
		
		std::vector<std::string> m_daysProcessed;
		void addDayUpdate(std::string yyyymmddimg);

	public:
		MasterCatalogue();
		~MasterCatalogue();
		
		bool settupFile(const char *tempPath, const char* fileCataloguePath) {
			m_viewFilePath->setRepositoryPath(fileCataloguePath);
			m_tempPath = tempPath;
			
			return m_viewFilePath->settup();
		}
		
		bool isFileEnabled() {
			return m_viewFilePath->isEnabled();
		}
		void setFileEnabled(bool b);

		void setFileTempPath(const char *path);
		void setFileFullSize(bool b);
		void setFilePreview1(bool b);
		void setFileThumbnail(bool b);

		bool isFileFullSize();
		bool isFilePreview1();
		bool isFilePreview2();
		bool isFilePreview3();
		bool isFileThumbnail();

		bool addImage(const char *sourcePath, MetadataObject &metadataObject, const char *imageRelativePath);
		bool addFileImage(const char *sourcePath, MetadataObject &metadataObject, const char *imageRelativePath);
	};

	class MasterWWWCatalogue {
		
		std::unique_ptr<ViewPath> m_viewWWWPath;
#ifdef WIN32
		std::unique_ptr<PageIndexMaker> m_pageIndexMaker;
		std::unique_ptr<ImagePageMaker> m_pageImageMaker;
#endif
		std::unique_ptr<SystemHistoryMaker> m_systemHistoryMaker;
		std::unique_ptr<ImportJournalMaker> m_inputJournalMaker;

		
		bool m_WWWEnabled;

		bool m_fullSize;
		bool m_preview1;
		bool m_preview2;
		bool m_preview3;
		bool m_thumbnail;

		bool m_fileFullSize;
		bool m_filePreview1;
		bool m_filePreview2;
		bool m_filePreview3;
		bool m_fileThumbnail;

		std::string m_tempPath;
		std::string m_archiveRootPath;
		bool createWWWPreviews(std::string& source, std::string& imageRelativePath);
		
		bool createWWWMetadata(MetadataObject& metadataObject, std::string& imageRelativePath);
		
		bool copyFile(const std::string& source, const std::string& dist, const std::string& file);
		bool copyFile(const std::string& source, const std::string& dist, const std::string& sfile, const std::string& dfile);
		bool convertRAW(const std::string& source, const std::string& dist, const std::string& fileOut);
		bool initWWW(const char* tempPath, const char* templatePath, const char* htmlPath);

		std::vector<std::string> m_daysProcessed;
		void addDayUpdate(std::string yyyymmddimg);
	public:
		MasterWWWCatalogue();
		~MasterWWWCatalogue();
		bool settupWWW(const char* tempPath, const char* templatePath, const char* wwwCataloguePath) {
			m_viewWWWPath->setRepositoryPath(wwwCataloguePath);
			if (initWWW(tempPath, templatePath, wwwCataloguePath) == false) {
				return false;
			}
			return m_viewWWWPath->settup();
		}
		
		bool settupSystemWWW(const char* archiveRootPath, const char* templatePath, const char* wwwCataloguePath, const char* systemHistoryPath, const char* inputJournalPath);

		bool isWWWEnabled() {
			return m_viewWWWPath->isEnabled();
		}

		void setWWWEnabled(bool b);

		void setWWWRepositoryPath(const char* pathToRepository) {
			m_viewWWWPath->setRepositoryPath(pathToRepository);
		}
		void setWWWTempPath(const char* path);
		void setWWWFullSize(bool b);
		void setWWWPreview1(bool b);
		void setWWWThumbnail(bool b);

		


		bool isWWWFullSize();
		bool isWWWPreview1();
		bool isWWWPreview2();
		bool isWWWPreview3();
		bool isWWWThumbnail();

		

	
		bool addWWWImage(const char* sourcePath, MetadataObject& metadataObject, const char* imageRelativePath);
		bool addFileImage(const char* sourcePath, MetadataObject& metadataObject, const char* imageRelativePath);
		bool makePages();
		bool processHistoryPages();
		bool processJournalPages();
		bool processWWWPages();
	};

}

