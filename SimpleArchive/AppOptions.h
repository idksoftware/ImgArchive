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
**	Filename	: AppOptions.h
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#ifndef ADMINAPPOPTIONS_H_
#define ADMINAPPOPTIONS_H_
#include <string>
#include <memory>
#include "argvparser.h"

namespace simplearchive {

/**
    @brief This class controls the commands input to the sia application.
    These commands are split into sub commands to sub section into areas of control.
    This sub commands are as follows:
 
    Import      Import(add) one or more images into the archve.
    Export      Export one or more images out of the archive.
    Checkout    Check-out one or more images out of the archive.
    Checkin     Check-in one or more images into the archive.
    UnCheckout  Uncheck out one or more images into the archive, any changes will be lost.
    Version     Show the current version/build of this software.
    View        Manages veiws that are generated from the archive.
    Uncheckin   ? this may not be used.
    Show        This shows the current state of archive properties.
    Cleanup	 Recursively clean up the workspace
    Relocate    Relocate images from one date to another
    Delete		Delete images
    Info        Same as show
    List        This may be superseded my show, list(ls) � List directory entries in the repository.
    Lock		Lock working images in the repository so that no other user can commit changes to them.
    Unlock      Unlock working images in the repository so that no other user can commit changes to them.
    History		Show image change hisory for an image or images
    Prop,       Show images properties.
    Unknown     An Unknown command has been entered so return control to the user.
    
    These sub commands are always the first argument on the command line,
    for example: the add commmand will be entered on the command line as follows:
        sia add --source-path="k:/my pictues"
 
    sia - Is the main command on the command line
    add - Is the first argument, this as the sub-command add, meaning that images are to
          be added into the archive.
    --source-path="k:/my pictues"
        - Is the last argument on the command line, this is an argument of add, it specifies the the source folder of where the image are to be found.
 
    The sub-commands are detailed as follows:
 
    Import      Import(add) one or more images into the archve.
    
    --source-path="<path of the folder where the images are to be found>"
                This is the path of the folder where the images are to be found.
                By default the sub-folders will be used to locate images.
    --sub-folders=<yes|no>
                Used to search sub folders. If sub-folders are not to be searched then use
                --sub-folders=no
 
    Export      Export one or more images out of the archive.
 
    --dist-path="<path to the where the images are to be exported>"
                This specifies where the exported files will be put. The format of the exported will be the same as the layout of the archive. By default, the whole of the archive will be exported.
    --to-date=<date>
                This specifies the date where the export will start exporting images.
 
    --from-date=<date>
                This specifies the date where the export will end xporting images.
 
    Checkout    Check-out one or more images out of the archive.
 
    --image-address
 
    --list
 
    --comment
 
    Checkin     Check-in one or more images into the archive.
 
    --image-address
 
    --list
 
    --comment
 
    UnCheckout  Uncheck out one or more images into the archive, any changes will be lost.
 
    --image-address
 
    --list
 
    --comment
 
    Version     Show the current version/build of this software.
 
 
    View        Manages veiws that are generated from the archive.
    Uncheckin   ? this may not be used.
 
    --image-address
 
    --list
 
    --comment
 
    Show        This shows the current state of archive properties.
 
 
    Cleanup	 Recursively clean up the workspace
    Relocate    Relocate images from one date to another
    Delete		Delete images
    Info        Same as show
    List        This may be superseded my show, list(ls) � List directory entries in the repository.
    Lock		Lock working images in the repository so that no other user can commit changes to them.
    Unlock      Unlock working images in the repository so that no other user can commit changes to them.
    History		Show image change hisory for an image or images
    Prop        Show images properties.
 
*/

class AppOptions {
public:
	typedef enum {
		CM_Import,      //< Import(add) one or more images into the archve.
		CM_Export,      //< Export one or more images out of the archive.
		CM_Checkout,    //< Check-out one or more images out of the archive.
		CM_Checkin,     //< Check-in one or more images into the archive.
		CM_UnCheckout,  //< Uncheck out one or more images into the archive, any changes will be lost.
		CM_Version,     //< Show the current version/build of this software.
		CM_View,        //< Manages veiws that are generated from the archive.
		CM_Uncheckin,   //< ? this may not be used.
		CM_Show,        //< This shows the current state of archive properties.
		CM_Cleanup,		// Recursively clean up the workspace
		CM_Relocate,	// Relocate images from one date to another
		CM_Delete,		// Delete images
        CM_Info,        // same as show
		CM_List,		// This may be superseded my show, list(ls) � List directory entries in the repository.
		CM_Lock,		// Lock working images in the repository so that no other user can commit changes to them.
		CM_Unlock,      // Unlock working images in the repository so that no other user can commit changes to them.
		CM_History,		// Show image change hisory for an image or images
		CM_Prop,        // Show images properties.
		CM_Unknown
	} CommandMode;

	typedef enum {
		SC_ShowCheckedOut,
		SC_ShowUncheckedOutChanges,
		SC_Unknown
	} ShowCommandOption;
private:
	static AppOptions *m_this;
	static bool m_list;
	static bool m_usingFile;
	static CommandMode m_commandMode;
	static std::string m_name;
	static std::string m_comment;
	static std::string m_imageAddress;
	static std::string m_distinationPath;
	static std::string m_filePath;
	ShowCommandOption m_showCommandOption;
	bool m_error;
	void setCommandMode(const AppOptions::CommandMode mode);
	bool setCommandMode(const char *modeString);
	void setName(const char *name);

	std::unique_ptr<CommandLineProcessing::ArgvParser> m_argvParser;
	AppOptions();
	
public:
	
	static AppOptions &get();

	virtual ~AppOptions();

    //* Initalise the command processing normally from the function main() arguments
	bool initalise(int argc, char **argv);
    //* Get the current sub command as an enum.
	AppOptions::CommandMode getCommandMode();
	const char *getName();
    //* Get the current sub command
	const char *getCommand();
    //* Get the current comment text
	const char *getComment();
    //* Get current image address
	const char *getImageAddress();
    //* Get distination path
	const char *getDistinationPath();
	//* return the pathe to an options file
	const char *getFilePath();
    //* Is the list option valid
	bool isList() { return m_list;  }
	bool isUsingFile() { return m_usingFile; }
    //* Ge The Show Command Option (only if the show command active).
	ShowCommandOption getShowCommandOption() { return m_showCommandOption; };
};

} /* namespace simplearchive */
#endif /* ADMINAPPOPTIONS_H_ */
