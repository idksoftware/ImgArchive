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

#ifndef ARCHIVEVISITOR_H_
#define ARCHIVEVISITOR_H_

namespace simplearchive {
/**
 * @brief This class is the functional class that class DirectoryVisitor uses as an action object.
 *
 * This class is a pure virtual object that on each new directory found a set of actions are
 * triggered as new file items are found.
 * This class object is passed into the constructor of the DirectoryVisitor class. when
 * required will need to clone itself using its make() function.
 */
class AVAction {
	friend class ArchiveVisitor;

protected:
        /// On the start of each directory found, this function is run.
	virtual void onStart() {};
		   /// At the end of each directory found, this function is run.
		virtual void onEnd() {};
	        /// On finding a file, this function is run.
		virtual void onYearFolder(const char *name) {};
	    /// On finding a file, this function is run.
		virtual void onYearEnd() {};
	        /// On finding a directory, this function is run.
		virtual void onDayFolder(const char *name) {};
	    /// On finding a directory, this function is run.
		virtual void onDayEnd() {};
		   /// On finding a directory, this function is run.
		virtual void onImage(const char *path, const char *name) {};

		virtual void onMetadata(const char *path, const char *name) {};
        /// This function is a factory function used to create new FolderVisitor objects.
	//virtual AVAction *make() { return new AVAction; }
public:
        /// Constructor
	AVAction() {};
        /// Distructor
	virtual ~AVAction() {};
};

/********************************************
	Test
 */
class TestAction : public AVAction {

protected:
        /// On the start of each directory found, this function is run.
	virtual void onStart() {
		//printf("onStart\n");
	};
	   /// At the end of each directory found, this function is run.
	virtual void onEnd() {
		//printf("onEnd\n");
	};
        /// On finding a file, this function is run.
	virtual void onYearFolder(const char *name) {
		//printf("onYearFolder %s: \n", name);
	};
    /// On finding a file, this function is run.
	virtual void onYearEnd() {
		//printf("onYearEnd\n");
	};
        /// On finding a directory, this function is run.
	virtual void onDayFolder(const char *name) {
		//printf("onDayFolder %s: \n", name);
	};
    /// On finding a directory, this function is run.
	virtual void onDayEnd() {
		//printf("onDayEnd\n");
	};
	   /// On finding a directory, this function is run.
	virtual void onImage(const char *path, const char *name) {
		//printf("\t\tonImage %s: \n", path);
	};

        /// This function is a factory function used to create new FolderVisitor objects.
	virtual TestAction *make() { return new TestAction; }
public:
        /// Constructor
	TestAction() {};
        /// Distructor
	virtual ~TestAction() {};
};

/**
 * @breif This class visits each file and directory starting from a root folder.
 *
 * This root folder is passed in the process function.
 * The class FolderVistor is used to create actions when the class visits directory items
 * such as files and directories.
*/
class ArchiveVisitor {
	AVAction *m_folderVisitor;
	std::string m_archivePath;
	std::string m_workspacePath;
	std::string m_dayPostfix;

	bool workspaceDay(std::string year);
	bool archiveDay(std::string year);
	bool processDay(std::string year);

public:
	/// This is the constructor that is passed the class that provides
	/// the actions
	ArchiveVisitor(AVAction *folderVisitor);
	ArchiveVisitor(AVAction *folderVisitor, const char *archivePath);
	ArchiveVisitor(AVAction *folderVisitor, const char *archivePath, const char *workspacePath);

	/// Distructor
	virtual ~ArchiveVisitor();
	/// This is the process function that processes the directory tree.
	/// @param rootFolder        The root folder which the visitor
	/// starts from.
	bool process(const char *rootFolder);
	bool workspace(const char *rootFolder);
	bool MasterCatalogue(const char *rootFolder);
	bool archive();
	void setPostfix(const char *s) {
		m_dayPostfix = s;
	}
};

} /* namespace simplearchive */
#endif /* ARCHIVEVISITOR_H_ */
