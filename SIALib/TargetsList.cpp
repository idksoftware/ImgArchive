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

#include "TargetsList.h"
#include "ImageExtentions.h"
#include "DirectoryVisitor.h"
#include "CLogger.h"
#include "HookCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	void ImageItem::processHook() {
		//printf("process Hook Item %s\n", m_path.c_str());
		OnFileCmd onFileCmd(m_path.c_str());
		onFileCmd.process();
	}

	/// This class creates a folder list for
	class FolderDir : public FolderVisitor {

		static int m_folderCount;
		static int m_fileCount;
		static ImageSets *m_imageSets;
		ImageSet *m_imageSet;
		std::string m_path;
	protected:
		virtual bool onStart(const char *path) {
			CLogger &logger = CLogger::getLogger();
			
			if (m_imageSets == 0) {
				m_imageSets = new ImageSets;
			}
			m_imageSet = new ImageSet(path);

			m_imageSets->insert(m_imageSets->end(), m_imageSet);
			logger.log(LOG_OK, CLogger::INFO, "Starting reading folder \"%s\"", path);
			//printf("==== Start ==== %d \n", m_folderCount++);
			return true;
		};

		virtual bool onFile(const char *path) {
			m_fileCount++;
			CLogger &logger = CLogger::getLogger();
			logger.log(LOG_OK, CLogger::INFO, "Reading file: %s", path);
			ImageExtentions& ie = ImageExtentions::get();
			std::string tmp = path;
			std::string filename = SAUtils::getFilename(tmp);
			if (ie.IsValid(filename.c_str()) == false) {
				logger.log(LOG_OK, CLogger::WARNING, "Not a valid file type \"%s\" rejecting ", filename.c_str());
				return true;
			}
			m_imageSet->insert(m_imageSet->end(), new ImageItem(path));
			return true;
		};
		virtual bool onDirectory(const char *path) {
			m_path = path;
			m_folderCount++;
			CLogger &logger = CLogger::getLogger();
			logger.log(LOG_OK, CLogger::INFO, "Now reading folder:  %s", m_path.c_str());
			return true;
		};
		virtual bool onEnd() {
			CLogger &logger = CLogger::getLogger();
			logger.log(LOG_OK, CLogger::INFO, "Ending reader folder %s", m_path.c_str());
			return true;
		};
		virtual FolderVisitor *make() {
			return new FolderDir;
		}
	public:
		FolderDir() {};
		virtual ~FolderDir() {};
		static ImageSets *getImageSets() {
			return m_imageSets;
		}
		static void destroy() {
			
			delete m_imageSets;
			m_imageSets = 0;
		}

		static int getFileCount() {
			return m_fileCount;
		}

		static int getFolderCount() {
			return m_folderCount;
		}
	};

	int FolderDir::m_folderCount = 0;
	int FolderDir::m_fileCount = 0;
	ImageSets *FolderDir::m_imageSets = 0;

	int TargetsList::getFileCount() {
		return FolderDir::getFileCount();
	}

	int TargetsList::getFolderCount() {
		return FolderDir::getFolderCount();
	}

	TargetsList::TargetsList()
	{
	}


	TargetsList::~TargetsList()
	{
	}

	void TargetsList::process(const char *path) {
		DirectoryVisitor directoryVisitor(new FolderDir);
		directoryVisitor.process(path);
	}

	

	ImageSets *TargetsList::getImageSets() {
		return FolderDir::getImageSets();
	}

	void TargetsList::destroy() {
		FolderDir::destroy();
	}
} /* namespace simplearchive */