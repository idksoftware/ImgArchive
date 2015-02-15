/*
 * MirrorManager.h
 *
 *  Created on: Nov 13, 2014
 *      Author: wzw7yn
 */

#ifndef MIRRORMANAGER_H_
#define MIRRORMANAGER_H_

#include <string>

namespace simplearchive {
//*

class MirrorList;
class MirrorManager {

	static std::string m_rootFolder;
	static std::string m_configFile;
public:
	MirrorManager();
	virtual ~MirrorManager();
	static bool initalise(const char *rootFolder, const char *configFile);
	/// This creates a mirror of the target root folder and all sub folders.
	/// using the mirrors defined in the configuration file passed in the
	/// initialise function
	static bool mirror();
	/// This synchronises the Mirror with the source (i.e. any missing files in
	/// the Mirror is copied from the source
	static bool SyncMirror();
	/// This checks the mirrors are in sync with the source directory.
	/// The source data is read and the mirror is checked that the file
	/// or directory exists. (only direct mirrors are used)
	static bool isMirrorInSync();
	/// This checks the source is in-sync with the mirror directory.
	/// The source data is read and the mirror is checked that the file
	/// or directory exists. (only direct mirrors are used)
	static bool isSourceInSync();
	/// Verifies that the mirrors are correct.
	static bool verifyMirror();
	/// Verifies that the Source data is correct.
	static bool verifySource();
};

} /* namespace simplearchive */
#endif /* MIRRORMANAGER_H_ */
