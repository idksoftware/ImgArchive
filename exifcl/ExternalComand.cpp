/*
 * CExif.cpp
 *
 *  Created on: May 28, 2014
 *      Author: wzw7yn
 */
#include <iostream>
#include "ExternalComand.h"
#include "ExternalShell.h"
#include "ConfigReader.h"
#include "SAUtils.h"
#include "ExternalExifMapper.h"
#include "ExifObject.h"
#include "CLogger.h"

namespace simplearchive {

/*! \exif_mainpage Shell and Configuration file components
*
* \section intro_sec Introduction
*
*	Sia provides a simple EXIF reader that can read EXIF data out of jpgs and possibly other image
*	types.  However EXIF data may be present in images that the internal reader may not be able to
*	decode.
*	There are external tools that may able to decode the EXIF information is images such as
*	ExifTool by Phil Harvey and Exiv2 project (www.exiv2.org). Sia enables these tools to be
*	integrated into the work-flow when adding images into the archive.
*	This extracted data is seamlessly inserted into the normal metadata that is archived with the image
*	as if, the internal reader extracted it.
*	Three external tools are supported :
*
*	Exiftool
*	Exiv2
*	Python Image Library.
*
*	The Python Image Library is a python library that can be used to extract EXIF SIA product provides a python
*	sample that intergrates into Sia. This camn be further extended as requited.
*
* \section external_tool_components_sec External Tool Components
*
* These components are used control external tools by a set of configuration files that provides the appropriate
* command line for the tool and key mapping for the output of the subsequently generated metadata. The output from
* an external command can be in two forms:
* 1) An External Output file that is read by SIA.
* 2) Reading the output from the external tool from the command line.
*
*
*
*
*
*
*
*/

ExternalComand::ExternalComand(const char *tempPath) {
	m_tempPath = tempPath;
}

ExternalComand::~ExternalComand() {
	// TODO Auto-generated destructor stub
}

bool ExternalComand::init(const char *externalCommandLine, const char *exifMapPath) {
	m_commandLine = externalCommandLine;
	m_exifMapPath = exifMapPath;
	return true;
}

// python exif.py [input] [output]
ExifObject *ExternalComand::process(const char *imagefile) {
	CLogger &logger = CLogger::getLogger();


	std::string out = m_tempPath;
	out += '/';
	out += SAUtils::getFilenameNoExt(imagefile);
	std::string in = imagefile;
	out.append(".exf");
	std::string cmd = replace(m_commandLine, in, out);
	logger.log(CLogger::FINE, "Using Exif command: \"%s\"", cmd.c_str());
	ExternalShell externalShell;
	externalShell.exec(cmd.c_str());
	logger.log(CLogger::FINE, "Exif command output: \"%s\"", externalShell.getOutput().c_str());
	ExternalExifMapper externalExifMapper;
	std::string exifMapFile(m_exifMapPath);
	exifMapFile += "/exif.ini";
	if (!externalExifMapper.init(exifMapFile.c_str())) {
		logger.log(CLogger::FINE, "Reading Exif map file \"%s\"", exifMapFile.c_str());
		ExifObject *exifObject = externalExifMapper.create(out.c_str());
		return exifObject;
	}
	
	return nullptr;
}

std::string ExternalComand::replaceToken(std::string &str, const char *toklabel, const char *repstr) {
	int s = str.find_first_of("[");
	if (s == -1) {
		return str;
	}
	std::string tok = toklabel;
	std::string tokstr = str.substr(s,tok.length());
	if (tokstr.compare(tok) != 0) {
		return str;
	}
	str.replace(s,tok.length(), repstr);
	return str;
}

std::string ExternalComand::replace(std::string &commandLine, std::string &in, std::string &out) {
	CLogger &logger = CLogger::getLogger();
	std::string command = commandLine;
	command = replaceToken(command, "[input]", in.c_str());
	command = replaceToken(command, "[output]", out.c_str());
	logger.log(CLogger::FINE, "Exif command line to be executed \"%s\"", command.c_str());
	return command;
}

} /* namespace simplearchive */