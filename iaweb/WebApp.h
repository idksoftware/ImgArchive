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
	class WebApp : public AppBase {
private:
	std::string m_HomePath;
	
public:
	/// Constructor
	WebApp();
	/// Destructor
	virtual ~WebApp();
	
	
	
	
	/// @brief Shows the configuration of the archive. All application processing starts here.
	bool Show();
protected:
	bool initaliseHomePath();
	bool initaliseConfig();
	bool initaliseArgs(int argc, char **argv);

	virtual bool doInitalise(int argc, char **argv);

	/// @brief This is the main application run function. All application processing starts here.
	bool doRun();

};

}
#endif /* APP_H_ */
