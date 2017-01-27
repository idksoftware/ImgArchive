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

/*
 Things to do??
 1) A reqect list of all the items completly rejected i.e files that have file exetentions
 this are unknown
 2) Dups rejected list.
 3) Post process phase where images can be post processed.
 3) Archive process phase. All images in the archive are visited for sum process to be applied.
 for example a new type of DNG format needs to be applied to all RAW images.
 or the databese need recreating.
 4) if run in a working directory look for images in that directory??
 5) make running without enviroment simple
 6) Update the dups index with the seq id so that if a dup is found then an option to use the original
 seg id can be used.




 6) create log file write points so the log is written during execution of processing.


 */
#ifndef APP_H_
#define APP_H_
#include "ArchiveBuilder.h"
#include "AppBase.h"

using namespace CommandLineProcessing;
namespace simplearchive {


/**
* @brief This is the main application class. This class is the first object main() will execute.
*/
	class AdminApp : public AppBase {
private:
	std::string m_HomePath;
	/**
		@brief This command creates a template archive. This includes the home configuration and the shadow
		archive. However the shadow archive will be create on the first addition of an image. The config file.
		will be updated with the location of both the shadow archive and the workspace.
		@param archivePath		Home path of archive.
		@param workspacePath	Workspace location.
		@param shadowPath		Shadow archive location.
	*/
	bool CreateArchive(const char *archivePath, const char *workspacePath, const char *shadowPath, bool users);
	
public:
	/// Constructor
	AdminApp();
	/// Destructor
	virtual ~AdminApp();
	
	bool initaliseConfig();
	/// @brief This is the main application run function. All application processing starts here.
	bool Run();
	/// @brief Shows the configuration of the archive. All application processing starts here.
	bool Show();
protected:
	virtual bool doInitalise(int argc, char **argv);
	bool doRun() { return true; };

};

}
#endif /* APP_H_ */
