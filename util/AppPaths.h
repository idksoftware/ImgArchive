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

#pragma once

#define SYSTEM_PATH					    "/system"
#define IMAGEID_FOLDER					"/system/imageid"
#define WORKSPACE_METADATA_PATH			"/.imga"
#define HOOKS_PATH						"/hooks"
#define BACKUP_DESTINATION_PATH			"/Image Backups"
#define MASTER_CATALOGUE_PATH			"/IAPictures"
#define MASTER_WWW_CATALOGUE_PATH		"/IAWebSite"
#define TOOLS_PATH						"/tools"
#define CONFIG_PATH						"/config"
#define DERIVATIVE_PATH					"/derivative"
#define MASTER_PATH						"/master"
#define HISTORY_PATH					"/history"
#define BACKUPS_PATH					"/backups"
#define METADATA_PATH					"/metadata"
#define IMAGEID_PATH					"/imageid"		// Sequence number
#define JOURNAL_PATH					"/journal"
#define MASTER_IMAGE_PATH				"/images"
#define DATABASE_PATH					"/db"
#define TEMP_PATH						"/tmp"
#define TEMPLATE_PATH					"/templates"
#define LOGS_PATH						"/logs"
#define INDEX_PATH						"/index"		// CRC index
#define PRIMARY_INDEX_PATH				"/pi"
#define DUPS_PATH						"/dups"
#define SQLITEDB_PATH					"/sqldb"
#define IMAGES_PATH						"/images"
#define LOG_PATH						"/logs"
#define BACKUP_PATH						"/backup"

#define DEFAULT_MASTER_CATALOGUE_PATH	"/IAPictures"
#define DEFAULT_PICTURES_PATH				"/Pictures"

#define DEFAULT_MASTER_BACKUP1_PATH		"/backup/MasterBackup1"
#define DEFAULT_MASTER_BACKUP2_PATH		"/backup/MasterBackup2"
#define DEFAULT_DERIVATIVE_BACKUP1_PATH	"/backup/DerivativeBackup1"
#define DEFAULT_DERIVATIVE_BACKUP2_PATH	"/backup/DerivativeBackup2"
#define MASTER_BACKUP1_PATH				"/MasterBackup1"
#define MASTER_BACKUP2_PATH				"/MasterBackup2"
#define DERIVATIVE_BACKUP1_PATH				"/DerivativeBackup1"
#define DERIVATIVE_BACKUP2_PATH				"/DerivativeBackup2"
constexpr auto DEFAULT_WORKSPACE_PATH = "/ImgArchive/Workspace";
constexpr auto DEFAULT_PICTURE_PATH = "/ImgArchive/Pictures";
constexpr auto DEFAULT_WWWIMAGE_PATH = "/ImgArchive/WWWPictures";
//#define DEFAULT_MASTER_PATH				"/IDK-Software/IMGA/master"
//#define DEFAULT_DERIVATIVE_PATH			"/IDK-Software/IMGA/derivative"
//#define DEFAULT_TOOLS_PATH				"/IDK-Software/IMGA/tools"
//#define DEFAULT_BACKUP_PATH				"/IDK-Software/IMGA/backup"
//#define DEFAULT_CONFIG_PATH				"/IDK-Software/IMGA/config"
//#define DEFAULT_DUPLICATE_PATH			"/IDK-Software/IMGA/dups"
//#define DEFAULT_HISTORY_PATH			"/IDK-Software/IMGA/history"
//#define DEFAULT_HOOKS_PATH				"/IDK-Software/IMGA/hooks"
//#define DEFAULT_LOGS_PATH				"/IDK-Software/IMGA/logs"
//#define DEFAULT_SYSTEM_PATH				"/IDK-Software/IMGA/system"
//#define DEFAULT_SYSTEM_PATH				"/IDK-Software/IMGA/templates"
#define DEFAULT_SOURCE_PATH				"/Pictures"
#define EXTERAL_EXIF_COMMAND_LINE		"\"[input]\""

#define CSVDB_PATH						"/csvdb"
#define CHKSTATUS_PATH					"/chkout"
#define SQLITE3_DB						"IASqlite3.db"

#define DEFAULT_HOME_PATH		   "/IDK-Software/ImgArchive"

#define IMGARCHIVE_HOME						"IMGARCHIVE_HOME"
#define IMGA_WORKSPACE						"IMGA_WORKSPACE"
#define IMGA_WORKSPACE_ENABLE				"IMGA_WORKSPACE_ENABLE"
#define IMGA_WORKSPACE_CHKOUT				"IMGA_WORKSPACE_CHKOUT"
#define IMGA_PICTURE						"IMGA_PICTURE"
#define IMGA_PICTURE_ENABLE					"IMGA_PICTURE_ENABLE"
#define IMGA_WWWIMAGE						"IMGA_WWWIMAGE"
#define IMGA_WWWIMAGE_ENABLE				"IMGA_WWWIMAGE_ENABLE"
#define IMGA_MASTER							"IMGA_MASTER"
#define IMGA_DERIVATIVE						"IMGA_DERIVATIVE"
#define IMGA_MASTER_BACKUP1					"IMGA_MASTER_BACKUP1"
#define IMGA_MASTER_BACKUP2					"IMGA_MASTER_BACKUP2"
#define IMGA_MASTER_BACKUP1_ENABLE			"IMGA_MASTER_BACKUP1_ENABLE"
#define IMGA_MASTER_BACKUP2_ENABLE			"IMGA_MASTER_BACKUP2_ENABLE"
#define IMGA_DERIVATIVE						"IMGA_DERIVATIVE"
#define IMGA_DERIVATIVE_BACKUP1				"IMGA_DERIVATIVE_BACKUP1"
#define IMGA_DERIVATIVE_BACKUP2				"IMGA_DERIVATIVE_BACKUP2"
#define IMGA_DERIVATIVE_BACKUP1_ENABLE		"IMGA_DERIVATIVE_BACKUP1_ENABLE"
#define IMGA_DERIVATIVE_BACKUP2_ENABLE		"IMGA_DERIVATIVE_BACKUP2_ENABLE"

constexpr auto DEFAULT_ALLUSER_HOME_PATH = "/IDK-Software/ImgArchive";
constexpr auto DEFAULT_ALLUSER_WWWIMAGE_PATH = "/IDK-Software/ImgArchive/IAWWWPictures";
constexpr auto DEFAULT_ALLUSER_PICTURE_PATH = "/ImgArchive/Pictures";
constexpr auto DEFAULT_ALLUSER_WORKSPACE_PATH = "/ImgArchive/Workspace";
constexpr auto DEFAULT_ALLUSER_MASTER_PATH = "/IDK-Software/ImgArchive/master";
constexpr auto DEFAULT_ALLUSER_DERIVATIVE_PATH = "/IDK-Software/ImgArchive/derivative";

constexpr auto DEFAULT_LOCAL_HOME_PATH = "/.ImgArchive";
constexpr auto DEFAULT_LOCAL_WWWIMAGE_PATH = "/.IAWWWPictures";
constexpr auto DEFAULT_LOCAL_PICTURE_PATH = "/ImgArchive/Pictures";
constexpr auto DEFAULT_LOCAL_WORKSPACE_PATH = "/ImgArchive/Workspace";
constexpr auto DEFAULT_LOCAL_MASTER_PATH = "/.ImgArchive/master";
constexpr auto DEFAULT_LOCAL_DERIVATIVE_PATH = "/.ImgArchive/derivative";
//constexpr autoEFAULT_WORKSPACE_PATH = "/IAWorkspace";
//m_sourcePath = tempHomeDrive + tempHomePath + "/Pictures";
//m_hookPath = m_homePath + "/hooks";

