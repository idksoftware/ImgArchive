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

#include <iostream>
#include "ExternalProcess.h"
#include "SAUtils.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

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
	*	The Python Image Library is a python library that can be used to extract EXIF ImgArchive product provides a python
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

	ExternalProcess::ExternalProcess() {
		// TODO Auto-generated constructor stub

	}

	ExternalProcess::~ExternalProcess() {
		// TODO Auto-generated destructor stub
	}

	bool ExternalProcess::init() {
		return true;
	}

	// python exif.py [input] [output]
	bool ExternalProcess::process(const char *imagefile) {
		return true;
	}

	std::string ExternalProcess::replaceToken(std::string &str, const char *toklabel, const char *repstr) {
		int s = str.find_first_of("[");
		if (s == -1) {
			return str;
		}
		std::string tok = toklabel;
		std::string tokstr = str.substr(s, tok.length());
		if (tokstr.compare(tok) != 0) {
			return str;
		}
		str.replace(s, tok.length(), repstr);
		return str;
	}

	std::string ExternalProcess::replace(std::string &commandLine, std::string &in, std::string &out) {

		std::string command = commandLine;
		command = replaceToken(command, "[input]", in.c_str());
		command = replaceToken(command, "[output]", out.c_str());
		//printf("cmd: %s", command.c_str());
		return command;
	}

} /* namespace simplearchive */
