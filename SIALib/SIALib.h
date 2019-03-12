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

#ifndef SIALIB_H_
#define SIALIB_H_

#include <string>
#include <memory>

#include "ExifDateTime.h"
#include "LogDocument.h"
/*
*/
namespace simplearchive {


	class ArchiveBuilder;
	class ArchiveObject;
	
	class CompletedSummary {
		std::string m_summary;
		std::string m_result;
	public:
		CompletedSummary() = default;
		virtual ~CompletedSummary() = default;
		void setSummary(const char *s) { m_summary = s; };
		const char *getSummary() { return m_summary.c_str(); };
		void setResult(const char *s) { m_result = s; };
		const char *getResult() { return m_result.c_str(); };
	};

	class SIALib
	{
	public:
		typedef enum {
			Workspace,		//* Initalise an archive with the default 
			Master,			//* Show
			Both			//* show version
		} Scope;
	private:
		std::string m_HomePath;
		std::shared_ptr<ArchiveBuilder> m_ArchiveBuilder;
		
		bool m_winsockRequired;
		bool m_socklibStarted;
		bool m_enableEvents;
		int m_udpPortNum;
		int m_tcpPortNum;
		bool m_enableServer;
		std::string m_udpAddress;
	public:
		SIALib();
		~SIALib();
		void enableEvents(const char *address, int port);
		void enableServer(int port);
		int initalise();
		int complete();
		//int complete();
		int checkin();

		bool Import();
		bool ImportFile(const char *filePath);
		void setForceDate();
		void setUseEXIFDate();
		void setUseFileDate();
		void setUseDateToday();
		void setUseDate(ExifDate &date);

		static const int getLastCode();
		
		static const char *getLastMessage();
		
		/**
		This shows the checked out images. The file path will be in the form:
		null = all archive
		year = all the year	i.e "2015"
		day  = all the day  i.e "2015-02-11"
		*/
		bool showCheckedOut(const char *addressScope);

		bool showUncheckedOutChanges(const char *addressScope);

		bool listContents(const char *addressScope);

		bool get(const char *filepath, const char *comment, bool force);

		bool checkout(const char *filepath, const char *comment, bool force);

		bool checkin(const char *filepath, const char *comment, bool force);
			
		bool uncheckout(const char *filepath, const char *comment, bool force);

		bool exportImage(const char *distpath);

		bool view(const char *name);

		bool show();

		bool status(const char *scope);

		bool mirror(const char *name);
		
		bool archive(const char *archivePath, const char *distPath, unsigned long sizeOfMedia, ExifDateTime *startDate, ExifDateTime *endDate);  //< Create backup volumes

		bool checkDisk();

		bool validate(const char *archivePath, const char *workspacePath, const char *homePath, Scope, bool repair = false);

		bool log(const char *filepath, LogDocument::FormatType& formatType);
	
	};

}
#endif /* SIALIB_H_ */
