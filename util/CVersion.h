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

#ifndef CVERSION_H_
#define CVERSION_H_
#include <string>

namespace simplearchive {

#define VERSION_EXT	".ver"
/// @brief This class controls the Versioning
class CVersion {

	/// Version number
	int m_version;

	/// Image name only (No extension)
	std::string m_nameonly;

	/// Version filename
	std::string m_versionName;

	/// Version filename
	std::string m_versionPath;

	/// Ordinal image filename
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

public:
	bool setToVersion(const char *name,int idx);
	bool setToVersion(int idx);
	/// Constructor
	CVersion(const char *name);

	///	Destructor
	virtual ~CVersion();

	/// New version pathname
	const char *newVersion();

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

	/// Copy the current data version to the highest version.
	bool CopyDataVersion2Old();

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
#endif /* CVERSION_H_ */
