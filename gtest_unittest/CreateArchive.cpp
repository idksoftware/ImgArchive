/*
 * CreateArchive.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: wzw7yn
 */

#include <string>
#include <iostream>
#include <fstream>
#include "SAUtils.h"
#include "CreateArchive.h"

namespace simplearchive {

// hook folder

const char *onfiledoc[] = {
#ifdef _WIN32
#else
	"#!/bin/ksh",
#endif
	"",
	"echo \"On file\"",
	"echo $hookScripts",
	"echo $ToolsPath",
	"echo $HookScripsPath",
	"echo $SourcePath",
	"echo $ArchivePath",

};

const char *exifdoc[] = {
	"# Exif File Name = Achive Name",
	"Color Space = ColorSpace",
	"Image Width = ExifImageWidth",
	"Image Height = ExifImageHeight",
	"Date / Time Original = DateTime",
	"#Compression",
	"#Depth",
	"#MIMEType",
	"#DigitalZoom",
	"#ExifVersion",
	"#ExposureProgram",
	"#GpsTimeStamp",
	"#LightSource",
	"#Page",
	"#PrimaryEncoding",
	"#Resolution",
	"#SampleColor",
	"#SensingMethod",
	"#ImageDescription",
	"#Make",
	"#Model",
	"#Orientation",
	"#BitsPerSample",
	"#Software",
	"#DateTime",
	"#DateTimeOriginal",
	"#DateTimeDigitized",
	"#SubSecTimeOriginal",
	"#Copyright",
	"#ExposureTime",
	"#FNumber",
	"#IsoSpeedRatings",
	"#ShutterSpeedValue",
	"#ExposureBiasValue",
	"#SubjectDistance",
	"#FocalLeng",
	"#FocalLengthIn35mm",
	"#Flash",
	"#MeteringMode",
	"#Latitude",
	"#Longitude",
	"#Altitude",
	"#ImageWidth",
	"#ImageLength",
	"#ColorSpace",
	"#ExifOffset",
	"#ExifImageWidth",
	"#DateTime",
	"#ImageHeight",
	"#SamplesPerPixel",
	"#Copyright = Copyright",
	"X Resolution = XResolution",
	"Y Resolution = YResolution",
	"#Artist = Artist"
};


// Config folder files
const char *configdoc[] = {
	"# The main configuration file #"
};

const char *mirrordoc[] = {
	"#name=mode. path, type",
	"#disk1=direct,/backup/home/tmp1,mirror"
};


const char *viewdoc[] = {
	"# name ,mode, path, type",
	"#default=dynamic,/backup/home/tmp1,link,readonly,include,/home/wzw7yn/sia/piclist.dat",
	"#link=dynamic,/backup/home/test/link,link,readonly",
	"#copy=dynamic,/backup/home/test/copy,copy,readonly"
};

const char *extdoc[] = {
	"nef:Raw:nikon-raw:Nikon RAW",
	"nrw:Raw:nikon-raw:Nikon RAW",
	"dng:Raw:nikon-raw:Adobe RAW",
	"raw:Raw:nikon-raw:Panasonic RAW",
	"raf:Raw:nikon-raw:Fuji RAW",
	"orf:Raw:nikon-raw:Olympus RAW",
	"srf:Raw:nikon-raw:Sony RAW",
	"sr2:Raw:nikon-raw:Sony RAW",
	"arw:Raw:nikon-raw:Sony RAW",
	"k25:Raw:nikon-raw:Kodak RAW",
	"kdc:Raw:nikon-raw:Kodak RAW",
	"dcr:Raw:nikon-raw:Kodak RAW",
	"dcs:Raw:nikon-raw:Kodak RAW",
	"drf:Raw:nikon-raw:Kodak RAW",
	"mos:Raw:nikon-raw:Leaf RAW",
	"pxn:Raw:nikon-raw:Logitech RAW",
	"crw:Raw:nikon-raw:Canon RAW",
	"cr2:Raw:nikon-raw:Canon RAW",
	"mrw:Raw:nikon-raw:Minolta RAW",
	"pef:Raw:nikon-raw:Pentax RAW",
	"ptx:Raw:nikon-raw:Pentax RAW",
	"mef:Raw:nikon-raw:Mamiya RAW",
	"3fr:Raw:nikon-raw:Hasselblad RAW",
	"fff:Raw:nikon-raw:Hasselblad RAW",
	"ari:Raw:nikon-raw:Arriflex RAW",
	"bay:Raw:nikon-raw:Casio RAW",
	"erf:Raw:nikon-raw:Epsom RAW",
	"cap:Raw:nikon-raw:Phase One RAW",
	"iiq:Raw:nikon-raw:Phase One RAW",
	"eip:Raw:nikon-raw:Phase One RAW",
	"rwl:Raw:nikon-raw:Leica RAW",
	"rwz:Raw:nikon-raw:Rawzor RAW",
	"srw:Raw:nikon-raw:Samsung RAW",
	"x3f:Raw:nikon-raw:Sigma RAW",
	"R3D:Raw:nikon-raw:RED RAW",
	"jpg:Picture:jpg:JPG",
	"png:Picture:png:PNG",
	"bmp:Picture:bmp:bmp",
	"gif:Picture:gif:gif",
	"tif:Picture:tif:tif",
	"tiff:Picture:tiff:tiff"
};

const char *readmedoc[] = {
		"WARNING: This directory (including its subdirectories) is used",
		"internally by SIA to maintain information about a SIA archive.",
		"You must not modify files in this directory directly.  You must",
		"not move or delete this directory without following the procedures",
		"described in the SIA documentation.  Failure to follow the",
		"procedures will cause SIA to fail."
};

CreateArchive::CreateArchive() {
	// TODO Auto-generated constructor stub

}

CreateArchive::~CreateArchive() {
	// TODO Auto-generated destructor stub
}

bool CreateArchive::all(const char *root) {
	if (makeFolders(root) == false) {
		return false;
	}
	if (createHookFiles(root, HOOKS_PATH) == false ) {
		return false;
	}
	if (createConfigFiles(root, CONFIG_PATH) == false ) {
		return false;
	}
	if (createReadme(root) == false ) {
		return false;
	}
	return true;
}

bool CreateArchive::makeFolders(const char *root) {

	if (makeFolder(root, CONFIG_PATH) == false) {
		return false;
	}
	if (makeFolder(root, TMP_PATH) == false) {
		return false;
	}
	if (makeFolder(root, DUPS_PATH) == false) {
		return false;
	}
	if (makeFolder(root, SQLITEDB_PATH) == false) {
		return false;
	}
	if (makeFolder(root, LOG_PATH) == false) {
		return false;
	}
	if (makeFolder(root, HISTORY_PATH) == false) {
		return false;
	}
	if (makeFolder(root, BACKUP_PATH) == false) {
		return false;
	}
	if (makeFolder(root, MASTER_PATH) == false) {
		return false;
	}
	if (makeFolder(root, TOOLS_PATH) == false) {
		return false;
	}
	if (makeFolder(root, HOOKS_PATH) == false) {
		return false;
	}
	if (makeFolder(root, TEMPLATE_PATH) == false) {
		return false;
	}
	return true;
}



bool CreateArchive::createConfigFiles(const char *root, const char *folder) {
	if (createFile(root, folder, "config.dat", (const char **)configdoc, (sizeof(configdoc)/sizeof(char *))) == false) {
		return false;
	}
	if (createFile(root, folder, "mirror.dat", (const char **)mirrordoc, (sizeof(mirrordoc)/sizeof(char *))) == false) {
		return false;
	}
	if (createFile(root, folder, "view.dat", (const char **)viewdoc, (sizeof(viewdoc)/sizeof(char *))) == false) {
		return false;
	}
	if (createFile(root, folder, "ext.dat", (const char **)extdoc, (sizeof(extdoc)/sizeof(char *))) == false) {
		return false;
	}
	if (createFile(root, folder, "exif.ini", (const char **)exifdoc, (sizeof(exifdoc)/sizeof(char *))) == false) {
		return false;
	}
	return true;
}

bool CreateArchive::createReadme(const char *root) {
	if (createFile(root, 0, "readme.txt", (const char **)readmedoc, (sizeof(readmedoc)/sizeof(char *))) == false) {
		return false;
	}
	return true;
}

bool CreateArchive::createHookFiles(const char *root, const char *folder) {
#ifdef _WIN32
	if (createFile(root, folder, "on-file-test.bat", (const char **)onfiledoc, (sizeof(onfiledoc)/sizeof(char *))) == false) {
#else
	if (createFile(root, folder, "on-file-test.sh", (const char **)onfiledoc, (sizeof(onfiledoc)/sizeof(char *))) == false) {
#endif
		return false;
	}

	return true;
}

bool CreateArchive::createFile(const char *root, const char *folder, const char *filename,
																		const char *array[],unsigned int size) {
	std::string fullPath = root;
	if (folder != 0) {
		fullPath += '/';
		fullPath += folder;
	}

	fullPath += '/';
	fullPath += filename;

	std::ofstream extFile(fullPath.c_str());
	extFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	if (!extFile.is_open()) {
		return false;
	}
	for (unsigned int i = 0; i < size; i++) {
		extFile << array[i] << '\n';
	}
	extFile.close();
	return true;
}

bool CreateArchive::makeFolder(const char *root, const char *folder) {
	std::string folderPath = root;
	folderPath += folder;
	if (SAUtils::DirExists(folderPath.c_str()) == false) {
		if (SAUtils::mkDir(folderPath.c_str()) == false) {
			return false;
		}
	}
	return true;
}

/*
bool CreateArchive::checkFolder(const char *root, const char *folder) {
	std::string folderPath = root;
	folderPath += folder;
	if (SAUtils::DirExists(folderPath.c_str()) == false) {
			return false;
	}
	return true;
}
*/
} /* namespace simplearchive */
