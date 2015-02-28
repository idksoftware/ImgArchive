#include "TargetsList.h"
#include "DirectoryVisitor.h"
#include "CLogger.h"
#include "HookCmd.h"

namespace simplearchive {

	void ImageItem::processHook() {
		//printf("process Hook Item %s\n", m_path.c_str());
		OnFileCmd onFileCmd(m_path.c_str());
		onFileCmd.process();
	}

	/// This class creates a folder list for
	class FolderDir : public FolderVisitor {

		static int m_count;
		static ImageSets *m_imageSets;
		ImageSet *m_imageSet;

	protected:
		virtual bool onStart(const char *path) {
			CLogger &logger = CLogger::getLogger();
			
			if (m_imageSets == 0) {
				m_imageSets = new ImageSets;
			}
			m_imageSet = new ImageSet(path);

			m_imageSets->insert(m_imageSets->end(), m_imageSet);
			logger.log(CLogger::INFO, "Starting reading folder \"%s\"%d", path, m_count++);
			//printf("==== Start ==== %d \n", m_count++);
			return true;
		};

		virtual bool onFile(const char *path) {
			CLogger &logger = CLogger::getLogger();
			logger.log(CLogger::INFO, "Reading file: %s", path);
			m_imageSet->insert(m_imageSet->end(), new ImageItem(path));
			return true;
		};
		virtual bool onDirectory(const char *path) {
			CLogger &logger = CLogger::getLogger();
			logger.log(CLogger::INFO, "Dir:  %s", path);
			return true;
		};
		virtual bool onEnd() {
			CLogger &logger = CLogger::getLogger();
			logger.log(CLogger::INFO, "Ending reader folder %d", m_count++);
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
			m_count = 0;
			delete m_imageSets;
			m_imageSets = 0;
		}
	};

	int FolderDir::m_count = 0;
	ImageSets *FolderDir::m_imageSets = 0;


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

	static void destroy() {
		FolderDir::destroy();
	}
} /* namespace simplearchive */