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

#ifndef CHECKDISK_H_
#define CHECKDISK_H_
#include <string>
#include <vector>
#include "SAUtils.h"
#include "ErrorCode.h"

namespace simplearchive {

typedef std::vector<std::string> SimpleImageList;

class ReportStatus {
public:
	typedef enum {
		Unknown = -1,
		ContentChanged,
		NameChanged,
		Missing,
		Added,
		CheckedOutNoChange,
		CheckedOutChanged,
		Unchanged,
	} Status;
private:
	Status m_status;
public:
	ReportStatus() { m_status = Unchanged; };
	ReportStatus(Status status) { m_status = status; };
	~ReportStatus() {};
	void set(Status &status) { m_status = status; };
	ReportStatus::Status get() { return m_status; }
	void set(const char *str);
	const char *toString();
};

/**
	This object  visits each file in the archive being validated. This object can report any error and
	fix them if that option has been selected.
*/
class VisitingObject {
	
public:
	VisitingObject() {};
	~VisitingObject() {};
	//< Process the file depending on file status.
	virtual bool process(const char *file, ReportStatus &status, const char *orginal = nullptr) = 0;
	virtual void startYear(const char *year) { printf("Visiting start Year %s", year);  };
	virtual void endYear(const char *year) { printf("Visiting end Year %s", year); };
	virtual void startDay(const char *day) { printf("Visiting start Day %s", day); };
	virtual void endDay(const char *day) { printf("Visiting end Day %s", day); };
};

/**
* @brief This class create a manifest file of the files in each folder.
* This can then be user to compare the file is the same folder to
* found if there is way changes. If there are changes that the system
* was not aware of then this me be an error that needs fixing by copying
* the changed file back from a mirror. 
* Also includes if the images has been checked out.
*/
class CkdskDiffFile;
class CheckDisk {
public:
	
private:
    /// Helper function to print a tag into a XML file.  
	std::string writeTag(const char *tag, const std::string& value, int tab);
    /// Helper function to print a tag into a XML file.
	std::string writeTag(const char *tag, const unsigned int value, int tab);
    /// The root archive path
	//static std::string m_archivePath;
	/// The root archive path
	static std::string m_derivativePath;
        /// Make an XML file manifest in the target directory 
	bool makeXML(const char *targetdir);
	bool makeXML(const char *chkdskFolder, const char *csvFile, const char *mxlFile);
	

	//std::string m_orginalName;
	//Error m_error;
	bool checkMissing(CkdskDiffFile &ckdskDiffFile, FileList_Ptr &filelist, VisitingObject *visitingObject, const char *address);
	bool checkExtra(CkdskDiffFile &ckdskDiffFile, FileList_Ptr &filelist, VisitingObject *visitingObject, const char *address);
public:
        /// Constructor
        /// @param archivePath  The root archive folder  
	//CheckDisk(const char *archivePath);
	//static void setArchivePath(const char *archivePath);
	CheckDisk() {};
        /// Destructor
	virtual ~CheckDisk();
        /// Makes the check disk data from the target folder
	bool makeCheckData(const char *targetdir);
    	/// Makes the check disk data from the target folder, saves into the save folder
	bool makeCheckData(const char *targetdir, const char *saveDir);
	bool makeCheckData(const char *chkdskfolder, const char *targetdir, const char *csvPath, const char *xmlPath);
        /// Checks the folder against the manifest file.
	//bool check(const char *targetdir);
	bool findNewImages(const char *archivePath, const char *targetPath, std::vector<std::string> &list);
	/// Checks the folder against the manifest file.
	/// @param targetdir		This is the path to the files to be checked.
	/// @param checkFilePath	This is the path to the manifest files
	/// @param address			This is the address of the images being checked
	/// @param reportingObject	This is the reorting object used to report the status of the checking
	bool check(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject& visitingObject);
	/// Write the status file
	//bool writeStatus(const char *path);
        /// Updates the manifest file with new file data.
	bool update(const char *rootPath, const char *targetdir, const char *targetfile);
	bool update(const char *chkdskFolderPath, const char *chkdskFilename, const char *targetFolderPath, const char *targetfilename);

	//const char *getOrginalName() {
	//	return m_orginalName.c_str();
	//}
	bool checkout(const char *filePath, const char *image);
	bool ischeckedOut(const char *filePath, const char *image);
	unsigned int getCrc(const char *filePath, const char *image);
	bool checkin(const char *filePath, const char *image);
	/// Thia function takes an empty list fills this list with any checked out images
	bool showCheckedOut(const char *filePath, SimpleImageList &imageList);
	
};

} /* namespace simplearchive */
#endif /* CHECKDISK_H_ */
