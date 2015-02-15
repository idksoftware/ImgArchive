/*
 * PreComitCmd.cpp
 *
 *  Created on: Aug 5, 2014
 *      Author: wzw7yn
 */
#include <stdlib.h>
#include "HookCmd.h"
#include "ExternalShell.h"
#include "SAUtils.h"
#include "SetEnv.h"
#include "ImagePath.h"
#include <fstream>
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
	setEnv.insert(setEnv.end(), EnvItem(PATH_TO_ARCHIVE, imagePath.getPathToArchive().c_str()));
	setEnv.insert(setEnv.end(), EnvItem(DATA_PATH, imagePath.getRelativePath().c_str()));
	setEnv.insert(setEnv.end(), EnvItem(METADATA_PATH, imagePath.getMetadataPath().c_str()));
	setEnv.insert(setEnv.end(), EnvItem(MAIN_METADATA_PATH, imagePath.getMainMetadataPath().c_str()));
	setEnv.process();
}

HookCmd::~HookCmd() {
}

void HookCmd::setHookPath(const char *path) {
	m_hookPath = path;
}

bool HookCmd::process() {

	//std::string out = SAUtils::getFilenameNoExt(imagefile);
	//std::string in = imagefile;
	std::string scriptName;
	if ((scriptName = getScriptNames()).empty() == true) {
		return false;
	}
	std::string cmd = m_hookPath + '/' + scriptName;

	ExternalShell externalShell;
	externalShell.exec(cmd.c_str());
	std::string output = externalShell.getOutput();
	//printf("Exec output: %s\n", output.c_str());
	return true;
}

std::string HookCmd::getScriptNames()
{
	if (readScriptsNames() == false) {
		return false;
	}
	if (SAUtils::DirExists(m_hookPath.c_str()) == false) {
		return false;
	}
	std::vector<std::string *> *filelist = SAUtils::getFiles(m_hookPath.c_str());
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		//printf("name %s\n", name->c_str());
		if (name->compare(".") == 0 || name->compare("..") == 0) {
			continue;
		}
		if (m_name.length() > name->length()) {
			continue;
		}
		std::string match = name->substr(0, m_name.length());
		if (m_name.compare(match) == 0) {
			return *name;
		}

	}
	std::string empty;
	return empty;
}

bool HookCmd::readScriptsNames() {
	switch(m_HookType) {
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
	case HC_Unknown:
		return false;
	default:
		return false;
	}
	return true;
}

OnFileCmd::OnFileCmd(const char *file) : HookCmd(HookCmd::HC_OnFile) {
	m_file = file;
};

bool OnFileCmd::process() {
	SetEnv setEnv;
	setEnv.insert(setEnv.end(), EnvItem(IMAGE_PATH, m_file.c_str()));

	setEnv.process();
	HookCmd::process();
	return true;
}

OnFolderCmd::OnFolderCmd(const char *folder) : HookCmd(HookCmd::HC_OnFolder) {
	m_folder = folder;
};
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
