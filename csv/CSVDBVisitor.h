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
class CSVDBAction {
	friend class CSVDBVisitor;

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
		virtual void onDBFile(const char *path, const char *name) {};
        /// This function is a factory function used to create new FolderVisitor objects.
	//virtual AVAction *make() { return new AVAction; }
public:
        /// Constructor
		CSVDBAction() {};
        /// Distructor
	virtual ~CSVDBAction() {};
};



/**
 * @breif This class visits each file and directory starting from a root folder.
 *
 * This root folder is passed in the process function.
 * The class FolderVistor is used to create actions when the class visits directory items
 * such as files and directories.
*/
class CSVDBVisitor {
	CSVDBAction *m_folderVisitor;
public:
	/// This is the constructor that is passed the class that provides
	/// the actions
	CSVDBVisitor(CSVDBAction *folderVisitor);
	/// Distructor
	virtual ~CSVDBVisitor();
	/// This is the process function that processes the directory tree.
	/// @param rootFolder        The root folder which the visitor
	/// starts from.
	bool process(const char *rootFolder);

};

} /* namespace simplearchive */
#endif /* ARCHIVEVISITOR_H_ */
