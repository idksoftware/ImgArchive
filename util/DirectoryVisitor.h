/*
 * DirectoryVisitor.h
 *
 *  Created on: Jun 6, 2014
 *      Author: wzw7yn
 */

#ifndef DIRECTORYVISITOR_H_
#define DIRECTORYVISITOR_H_

#include <string>
#include <vector>

namespace simplearchive {

class FolderItem;
class DirNode;
class DirectoryVisitor;
/// This class is used by the DirectoryVisitor. The DirectoryVisitor class will
/// call the make function to make FolderVisitor objects when required.
/// This must be used as a base class for new classes that the DirectoryVisitor uses to
/// process folders the DirectoryVisitor class visits.
///
class FolderVisitor {
	friend class DirectoryVisitor;
	friend class DirNode;
protected:
	virtual bool onStart(const char *path) { return true; };
	virtual bool onFile(const char *path) { return true; };
	virtual bool onDirectory(const char *path) { return true; };
	virtual bool onEnd() { return true; };
	virtual FolderVisitor *make() { return new FolderVisitor; }
public:

	FolderVisitor() {};
	virtual ~FolderVisitor() {};
};

class DirectoryVisitor {
	DirNode *m_dirNode;
	FolderVisitor *m_folderVisitor;
	bool m_deleteFolderVisitor;
public:
	/// Constructor
	/// @parm folderVisitor - pointer to FolderVisitor
	DirectoryVisitor(FolderVisitor *folderVisitor, bool val = true);

	// Destructor
	virtual ~DirectoryVisitor();
	/// This Function processes the files under the root using the
	/// FolderVisitor class passed in the constructor
	bool process(const char *rootFolder);
	void deleteFolderVisitor(bool val = true) {
		m_deleteFolderVisitor = val;
	}
};

} /* namespace simplearchive */
#endif /* DIRECTORYVISITOR_H_ */
