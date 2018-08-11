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
	virtual std::shared_ptr<FolderVisitor> make() { return (std::make_shared<FolderVisitor>()); }
public:

	FolderVisitor() {};
	virtual ~FolderVisitor() {};
};

class DirectoryVisitor {
	std::shared_ptr<DirNode> m_dirNode;
	std::shared_ptr<FolderVisitor> m_folderVisitor;
	bool m_deleteFolderVisitor;
public:
	/// Constructor
	/// @parm folderVisitor - pointer to FolderVisitor
	DirectoryVisitor(std::shared_ptr<FolderVisitor> folderVisitor, bool val = true);

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

