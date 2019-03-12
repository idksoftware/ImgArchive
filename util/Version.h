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

#include <string>
#include <sstream>
#include <iomanip>
#include "ExifDate.h"

namespace simplearchive {

#define VERSION_EXT	".ver"
/// @brief This class controls the Versioning
class Version {

	/// Version number
	int m_version;

	/// Image name only (No extension)
	std::string m_nameonly;

	/// Version filename
	std::string m_versionName;

	/// Version filename
	std::string m_versionPath;

	/// Original image filename
	std::string m_imagefilename;

	/// Ordinal image path
	std::string m_imagePath;

	/// Version folder
	std::string m_versionFolder;

	/// Split ordinal path
	bool splitpath(const char *name);

	/// Last version number
	int fileLastVersion(const char *name);

	/// Set name only
	std::string nameOnly(const char *name);

	/// Verson index from string
	int versionIndex(const char *name);

	/// Set to version
	std::string m_shortRelPath;

	/// Set to version
	std::string m_sourcePath;

	/// Set to version
	std::string m_encodedVersion;

	std::string m_ext;
public:
	bool setToVersion(const char *name, int idx);
	bool setToVersion(int idx);

	// This will tacke the path of the image to be added to the derivative database and 
	// create the full path for inclusion into the derivative database based on todays
	// date
	Version(const char *root, const char *sourcePath, const char *name, int version) {
		ExifDate date;
		date.now();
		int year = date.getYear();
		int month = date.getMonth();
		int day = date.getDay();
		std::stringstream s;
		s << year << '-' << std::setw(2) << std::setfill('0') << month
			<< '-' << std::setw(2) << std::setfill('0') << day;
		m_shortRelPath = s.str();
		m_version = version;
		m_sourcePath = sourcePath;
		m_imagefilename = name;
		m_nameonly = nameOnly(name);
		std::string versionName = makeVersion();
		m_versionPath = root;
		m_versionPath += '/';
		m_versionPath += std::to_string(year);
		m_versionPath += '/';
		m_versionPath += m_shortRelPath;
		m_versionPath += '/';
		m_versionPath += versionName;
	};

	virtual ~Version() {};

	/// New version pathname
	const char *makeVersion();

	const char *makeEncodedVersion();
	/// Create new version metadata filename using current version index
	const char *newVersionMetadata(const char *name);
	/// Get Version number
	int getVersion() const {
		return m_version;
	}

	/// Set number
	void setVersion(int version) {
		m_version = version;
	}

	/// Revert the current version which may have been
	/// edited in error.
	int Revert();
	/// Get version name
	const std::string& getVersionName() const {
		return m_versionName;
	}

	bool HasChanged();

	/// Get version name
	const std::string& getVersionPath() const {
		return m_versionPath;
	}

	/// Get image name only (No extension)
	const std::string& getNameonly() const {
		return m_nameonly;
	}

	const std::string& getShortRelPath() {
		return m_shortRelPath;
	}

	const std::string& getSourcePath() {
		return m_sourcePath;
	}
	/// Copy the current data version to the highest version.
	bool checkin(const char *workspace, const char *database);

	/// Copy the current data version to the highest version.
	bool checkout(const char *workspace, const char *database);
	/// Copy the current data version to the temp version version.
	bool CopyDataVersion2Temp();

	/// Copy the new version to the data folder
	bool CopyNewVersion2Data(const char *newImage);

	/// Copy the version in data folder to current version
	/// This is used to uncheckout
	bool CopyDataVersion2Current();


	/// Remove temp
	bool RemoveTemp();

};

} /* namespace simplearchive */

