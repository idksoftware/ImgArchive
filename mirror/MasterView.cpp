#include "MasterView.h"
#include "SAUtils.h"
#include "AppPaths.h"
#include "pathcontroller.h"
#include "HookCmd.h"


namespace simplearchive {
	MasterView::MasterView() : m_viewPath(std::make_unique<ViewPath>())
	{
		m_fullSize = true;
		m_preview1 = true;
		m_thumbnail = false;
	}


	MasterView::~MasterView()
	{
	}

	void MasterView::setTempPath(const char *path) {
		m_tempPath = path;
	}

	bool MasterView::addImage(const char *sourcePath, const char *imageRelativePath) {
		std::string yyyymmddStr = imageRelativePath;
		if (m_viewPath->settupRelative(yyyymmddStr) == false) {
			return false;
		}
		std::string dist = m_viewPath->getImagePath();
		std::string source = sourcePath;
		std::string file = m_viewPath->getImageName();
		
		if (isFullSize() == true) {
			if (copyFile(source, dist, file) == false) {
				return false;
			}
		}
		if (isPreview1() == true || isThumbnail() == true) {
			if (createPreviews(source, yyyymmddStr) == false) {
				return false;
			}
		}
		return true;
	}

	bool MasterView::createPreviews(std::string &sourcePath, std::string &yyyymmddStr) {

		if (m_viewPath->settupRelative(yyyymmddStr) == false) {
			return false;
		}

		std::string dist = m_tempPath;
		std::string source = sourcePath;
		std::string file = m_viewPath->getImageName();
		if (copyFile(source, dist, file) == false) {
			return false;
		}
		std::string name = file.substr(0, file.find_last_of("."));
		std::string ext = file.substr(file.find_last_of(".") + 1);

		if (isPreview1() == true) {
			std::string preview1Name = name;
			preview1Name += "_1.";
			preview1Name += ext;
			//simplearchive::HookCmd::setHookPath("C:\\temp\\cmds");

			OnViewPreview1Cmd onFolderCmd("C:\\temp\\2017\\2017-08-12\\pic1.png");

			onFolderCmd.process();
			if (copyFile(m_tempPath, m_viewPath->getImagePath(), file, preview1Name) == false) {
				return false;
			}

		}
		if (isThumbnail() == true) {

		}
		return true;
	}

	
	bool MasterView::copyFile(const std::string &source, const std::string &dist, const std::string &file) {
		
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

	bool MasterView::copyFile(const std::string &source, const std::string &dist, const std::string &sfile, const std::string &dfile) {

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

	void MasterView::setFullSize(bool b) {
		m_fullSize = b;
	}
	void MasterView::setPreview1(bool b) {
		
		m_preview1 = b;
	}
	void MasterView::setThumbnail(bool b) {
		
		m_thumbnail = b;
	}

	bool MasterView::isFullSize() {
		return m_fullSize;
	}
	bool MasterView::isPreview1() {
		return m_preview1;
	}
	bool MasterView::isThumbnail() {
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
		m_yyyymmddStr = yyyymmddStr;

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
		m_imagePath = m_yyyymmddStrPath + MASTER_DATA_PATH;
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
		m_enabled = true;
	}

	std::string  ViewPath::getRepositoryPath() const {
		return m_pathToRepository;
	}
}