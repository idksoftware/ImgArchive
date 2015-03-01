/*
* ReadConfig.h
*
*  Created on: May 7, 2014
*      Author: wzw7yn
*/

#ifndef SIALIB_H_
#define SIALIB_H_

#include <string>
/*
*/
namespace simplearchive {


	class ArchiveBuilder;
	class SIALib
	{
		std::string m_HomePath;
		std::auto_ptr<ArchiveBuilder> m_ArchiveBuilder;
	public:
		SIALib();
		~SIALib();
		int initalise();
		int complete();
		int checkin();

		bool Import();

		bool checkout(const char *filepath, const char *comment);

		bool checkin(const char *filepath, const char *comment);
			
		bool uncheckout(const char *filepath, const char *comment);
			
		bool view(const char *name);

		bool mirror(const char *name);
		
		bool archive();  //< Create backup volumes

	};

}
#endif /* SIALIB_H_ */