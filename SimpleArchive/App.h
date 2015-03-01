/*
 * App.h
 *
 *  Created on: Feb 11, 2014
 *      Author: wzw7yn

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
//#include "SIALib.h"

namespace simplearchive {

class ArchiveBuilder;
/**
* @brief This is the main application class. This class is the first object main() will execute.
*/
class App {
private:
	std::string m_HomePath;
	ArchiveBuilder m_ArchiveBuilder;
	
	
public:
	/// Constructor
	App();
	/// Destructor
	virtual ~App();

	bool initalise(int argc, char **argv);
	/// @brief This is the main application run function. All application processing starts here.
	bool Run();
};

}
#endif /* APP_H_ */
