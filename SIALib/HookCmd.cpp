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

#include <stdlib.h>
#include "CLogger.h"
#include "HookCmd.h"
#include "ExternalShell.h"
#include "SAUtils.h"
#include "SetEnv.h"
#include "ImagePath.h"
#include <fstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	std::string HookCmd::m_hookPath;

	HookCmd::HookCmd(HookType type) {
		m_HookType = type;

	}


	void HookCmd::init(ImagePath &imagePath) {
		SetEnv setEnv;

		setEnv.insert(setEnv.end(), EnvItem(RELATIVE_PATH, imagePath.getRelativePath().c_str()));
		setEnv.insert(setEnv.end(), EnvItem(IMAGE_PATH, imagePath.getImagePath().c_str()));
		setEnv.insert(setEnv.end(), EnvItem(YEAR, imagePath.getYearStr().c_str()));
		setEnv.insert(setEnv.end(), EnvItem(YYYYMMDD, imagePath.getYyyymmddStr().c_str()));
		setEnv.insert(setEnv.end(), EnvItem(PATH_TO_ARCHIVE, imagePath.getPathToWorkspace().c_str()));
		setEnv.insert(setEnv.end(), EnvItem(DATA_PATH, imagePath.getRelativePath().c_str()));
		setEnv.insert(setEnv.end(), EnvItem(METADATA_PATH_LABLE, imagePath.getLocalMasterMetadataPath().c_str()));
		setEnv.insert(setEnv.end(), EnvItem(MAIN_METADATA_PATH, imagePath.getMainMetadataPath().c_str()));
		setEnv.process();
	}

	HookCmd::~HookCmd() {
	}

	void HookCmd::setHookPath(const char *path) {
		m_hookPath = path;
	}

	bool HookCmd::process() {
		return processAll(nullptr, nullptr, nullptr);
	}
	
	bool HookCmd::process(const char *arg1) {
		return processAll(arg1, nullptr, nullptr);
	}
	
	bool HookCmd::process(const char *arg1, const char *arg2) {
		return processAll(arg1, arg2, nullptr);
	}

	bool HookCmd::process(const char *arg1, const char *arg2, const char *arg3) {
		return processAll(arg1, arg2, arg3);
	}

	bool HookCmd::processAll(const char* arg1, const char* arg2, const char* arg3) {
		CLogger& logger = CLogger::getLogger();
		std::string scriptName;
		if ((scriptName = getScriptNames()).empty() == true) {
			return false;
		}
		
		std::string ext = SAUtils::getExtention(scriptName.c_str());
		std::string cmd;
		if (ext.compare("py") == 0) {
			cmd = "python.exe ";
		}
		cmd += m_hookPath + '/' + scriptName;
		if (SAUtils::FileExists(cmd.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::TRACE, "Cannot find ");
			return false;
		}
		if (arg1 != nullptr) {
			cmd += ' '; cmd += arg1;
		}
		if (arg2 != nullptr) {
			cmd += ' '; cmd += arg2;
		}
		if (arg3 != nullptr) {
			cmd += ' '; cmd += arg3;
		}
		// = "g:\\sia\\archive\\tools\\convert dsc_3248.nef -quality 100 dsc_3248.jpg";
		ExternalShell externalShell;
		if (externalShell.exec(cmd.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::TRACE, "hook process failed with output: %s", m_output.c_str());
		}
		m_output = externalShell.getOutput();
		logger.log(LOG_OK, CLogger::Level::TRACE, "hook process output: %s", m_output.c_str());
		return true;
	}

	std::string HookCmd::getScriptNames()
	{
		std::string empty;
		// Replace with exception 
		if (readScriptsNames() == false) {
			return empty;
		}
		// Replace with exception 
		if (SAUtils::DirExists(m_hookPath.c_str()) == false) {
			return empty;
		}
		FileList_Ptr filelist = SAUtils::getFiles_(m_hookPath.c_str());

		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string name = *i;
			//printf("name %s\n", name->c_str());
			if (name.compare(".") == 0 || name.compare("..") == 0) {
				continue;
			}
			if (m_name.length() > name.length()) {
				continue;
			}
			std::string match = name.substr(0, m_name.length());
			if (m_name.compare(match) == 0) {
				return name;
			}

		}

		return empty;
	}

	bool HookCmd::readScriptsNames() {
		switch (m_HookType) {
		case HC_OnFile:
			m_name = "on-file";
			break;
		case HC_OnFolder:
			m_name = "on-folder";
			break;
		case HC_OnContainer:
			m_name = "on-image-set";
			break;
		case HC_PostArchive:
			m_name = "post-archive";
			break;
		case HC_PreArchive:
			m_name = "pre-archive";
			break;
		case HC_PreProcess:
			m_name = "post-process";
			break;
		case HC_OnFileCopy:
			m_name = "copy-file";
			break;
		case MC_MVPreview1:
			m_name = "view-preview1";
			break;
		case MC_MVPreview2:
			m_name = "view-preview2";
			break;
		case MC_MVPreview3:
			m_name = "view-preview3";
			break;
		case MC_MVThumb:
			m_name = "view-thumbnail";
			break;
		case MC_MVRAW:
			m_name = "from-RAW";
			break;
		case MC_MVImageSize:
			m_name = "ident-size";
			break;
		case HC_Unknown:
			return false;
		default:
			return false;
		}
		return true;
	}

	OnFileCmd::OnFileCmd(const char *file) : HookCmd(HookCmd::HC_OnFile) {
		m_path = file;
	};

	bool OnFileCmd::process() {
		SetEnv setEnv;

		setEnv.insert(setEnv.end(), EnvItem(IMAGE_PATH, m_file.c_str()));
		std::string ext = SAUtils::getExtention(m_path);
		std::string file = SAUtils::getFilenameNoExt(m_path);
		setEnv.process();
		HookCmd::process(file.c_str(), ext.c_str());
		return true;
	}

	OnFileCopyCmd::OnFileCopyCmd(const char *path, const char *image) : HookCmd(HookCmd::HC_OnFileCopy) {
		m_path = path;
		m_image = image;
	};

	bool OnFileCopyCmd::process() {
		SetEnv setEnv;
		setEnv.insert(setEnv.end(), EnvItem(IMAGE_PATH, m_path.c_str()));
		setEnv.insert(setEnv.end(), EnvItem(IMAGE_NAME, m_image.c_str()));
		setEnv.process();
		HookCmd::process();
		return true;
	}

	OnFolderCmd::OnFolderCmd(const char *folder) : HookCmd(HookCmd::HC_OnFolder) {
		m_folder = folder;
	};

	OnIndentSizeCmd::OnIndentSizeCmd(const char *imageName) : HookCmd(HookCmd::MC_MVImageSize) {
		m_imageName = imageName;
	};

	bool OnIndentSizeCmd::process() {
		return HookCmd::process(m_imageName.c_str());
	};

	OnViewRAWCmd::OnViewRAWCmd(const char *sourceFilePath, const char *distFilePath) : HookCmd(HookCmd::MC_MVRAW) {
		m_sourceFilePath = sourceFilePath;
		m_distFilePath = distFilePath;
	}

	bool OnViewRAWCmd::process() {
		return HookCmd::process(m_sourceFilePath.c_str(), m_distFilePath.c_str());
	}

	OnViewThumbnailCmd::OnViewThumbnailCmd(const char *sourceFilePath, const char *distFilePath) : HookCmd(HookCmd::MC_MVPreview1) {
		m_sourceFilePath = sourceFilePath;
		m_distFilePath = distFilePath;
	}

	bool OnViewThumbnailCmd::process() {
		return HookCmd::process(m_sourceFilePath.c_str(), m_distFilePath.c_str());
	}
	
	OnViewPreview1Cmd::OnViewPreview1Cmd(const char *sourceFilePath, const char *distFilePath) : HookCmd(HookCmd::MC_MVPreview1) {
		m_sourceFilePath = sourceFilePath;
		m_distFilePath = distFilePath;
	}

	bool OnViewPreview1Cmd::process() {
		return HookCmd::process(m_sourceFilePath.c_str(), m_distFilePath.c_str());
	}

	OnViewPreview2Cmd::OnViewPreview2Cmd(const char *sourceFilePath, const char *distFilePath) : HookCmd(HookCmd::MC_MVPreview2) {
		m_sourceFilePath = sourceFilePath;
		m_distFilePath = distFilePath;
	}

	bool OnViewPreview2Cmd::process() {
		return HookCmd::process(m_sourceFilePath.c_str(), m_distFilePath.c_str());
	}

	OnViewPreview3Cmd::OnViewPreview3Cmd(const char *sourceFilePath, const char *distFilePath) : HookCmd(HookCmd::MC_MVPreview3) {
		m_sourceFilePath = sourceFilePath;
		m_distFilePath = distFilePath;
	}

	bool OnViewPreview3Cmd::process() {
		return HookCmd::process(m_sourceFilePath.c_str(), m_distFilePath.c_str());
	}
//OnFolderCmd::OnFolderCmd(ImagePath &imagePath) : HookCmd(HookCmd::HC_OnFolder, imagePath) {}

//PostArchiveCmd::PostArchiveCmd() : HookCmd(HookCmd::HC_OnContainer) {};
//PostArchiveCmd::PostArchiveCmd(ImagePath &imagePath) : HookCmd(HookCmd::HC_OnContainer, imagePath) {}


PostArchiveCmd::PostArchiveCmd() : HookCmd(HookCmd::HC_PostArchive) {};
PostArchiveCmd::PostArchiveCmd(ImagePath &imagePath) : HookCmd(HookCmd::HC_PostArchive) {
	init(imagePath);
}

PreArchiveCmd::PreArchiveCmd() : HookCmd(HookCmd::HC_PreArchive) {};
PreArchiveCmd::PreArchiveCmd(ImagePath &imagePath) : HookCmd(HookCmd::HC_PreArchive) {
	init(imagePath);
}


PreProcessCmd::PreProcessCmd(): HookCmd(HookCmd::HC_PreProcess) {};
PreProcessCmd::PreProcessCmd(ImagePath &imagePath) : HookCmd(HookCmd::HC_PreProcess) {
	init(imagePath);
}

} /* namespace simplearchive */
