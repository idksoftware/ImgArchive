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
**	@File		siaglobal.h
**	@Author		I.Ferguson
**	@Version	1.000
**	@Date		26-05-2015
**
** #$$@@$$# */

#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#ifdef _WIN32
#include <crtdbg.h>
#endif

#ifdef _DEBUG
#ifndef DEBUG_NEW
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif
#endif  // _DEBUG

#define EXIFOBJECT_CPP				2
#define EXTERNALCOMAND_CPP			3
#define EXTERNALEXIFMAPPER_CPP		4
#define CHECKDISK_CPP				5
#define CONFIGREADER_CPP			6
#define FILEINFO_CPP				7
#define VIEWMANAGER_CPP				8
#define APPCONFIG_CPP				9
#define ARCHIVEBUILDER_CPP			10
#define ARCHIVEOBJECT_CPP			11
#define BASICMETADATAFACTORY_CPP	12
#define HOOKCMD_CPP					13
#define IMAGECONTAINER_CPP			14
#define IMAGEFILEREADER_CPP			15
#define IMAGEINDEX_CPP				16
#define MASTERCATALOGUE_CPP			17
#define PAGEMAKER_CPP				18
#define SIAAPPLICATIONSTATE_CPP		19
#define SIALIB_CPP					20
#define TARGETSLIST_CPP				21
#define VERSIONCONTROL_CPP			22
#define VIEWmANAGER_CPP				23





#ifndef FILECODE
#define FILECODE 01
#endif

#define LOG_OK ((FILECODE * 10000) + __LINE__)

#define LOG_DUPLICATE			10
#define LOG_INVALID_FILE_TYPE	11
#define LOG_WORKSPACE_NOT_FOUND	12
#define LOG_UNABLE_CREATE_WWW_IMAGES	13
#define LOG_UNABLE_CREATE_FILE_IMAGES	14


#define LOG_STARTING			30
#define LOG_ANALISING			31
#define LOG_IMPORTING			32
#define LOG_INITALISATION		33 // Initalisation
#define LOG_COMPLETED   		34 // Application completed successfully
#define LOG_SOURCE_PATH   		35 // Application completed successfully
#define LOG_ASSOCIATING			36	// Associating
#define LOG_ARCHIVING_IMAGE		37  // Archiving image

#define LOG_INITIAL_SUMMARY		101
#define LOG_IMAGE_SUMMARY		102
#define LOG_FOLDER_SUMMARY		103
#define LOG_CURRENT_IMAGE		104
#define LOG_IMAGE_NAME			105
#define LOG_COMPLETED_SUMMARY	106

