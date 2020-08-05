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

#include "ExifDate.h"
#include "ResultsPresentation.h"
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

		enum class MainArchives {
			Master,			//* Show
			Derivative,			//* Show
			Both
		};

		enum class Scope {
			Workspace,		//* Initalise an archive with the default 
			Master,			//* Show
			Derivative,			//* Show
			All,			//* Show
			Main			//* show version
		};

		enum class VerifyBackups {
			Backup_1,		//* Initalise an archive with the default 
			Backup_2,			//* Show
			Both,		//* Show
			None
		};

		enum class Backups {
			Backup_1,		//* Initalise an archive with the default 
			Backup_2,			//* Show
			Both
		};

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
		void setUseDate(const ExifDate &exifDate);

		static const int getLastCode();
		
		static const char *getLastMessage();
		
		/**
		This shows the checked out images. The file path will be in the form:
		null = all archive
		year = all the year	i.e "2015"
		day  = all the day  i.e "2015-02-11"
		*/
		bool status(const char* scope, ResultsPresentation::FormatType& formatType, const char* filepath);

		bool showCheckedOut(const char* scope, ResultsPresentation::FormatType& formatType, const char* filepath);

		bool showUncheckedOutChanges(const char* scope, ResultsPresentation::FormatType& formatType, const char* filepath);

		bool listContents(const char *addressScope); // maybe not used

		bool get(const char *filepath, const char *comment, bool force);

		bool checkout(const char *scope, const char *comment, bool force);
		
		bool checkin(const char * scope, const char *comment, bool force);
		
		bool uncheckout(const char* scope, const char* comment, bool force);
		
		bool exportImage(const char *distpath);

		// Not_Used bool view(const char *name);

		bool show();

		bool deleteImages(const char* scope, const char* comment, bool force);
		
		bool undeleteImages(const char* scope, const char* comment);
		
		bool purge();

		bool metadata(const char* scope, const char* formatType, const char* filePath);

		bool setProperty(const char* scope, const char* option, const char* value);

		bool workspace(bool sync);

		bool metadataTemplate(bool repo, bool templateFile);

		bool mirror(const char *name);
		
		bool archive(const char *archivePath, const char *distPath, unsigned long sizeOfMedia, ExifDateTime *startDate, ExifDateTime *endDate);  //< Create backup volumes

		bool checkDisk();

		bool validate(Scope scope, bool main, VerifyBackups verifyBackups, bool repair);

		bool sync(MainArchives mainArchives, Backups backups);

		bool log(const char *filepath, const char* fromDate, const char* toDate, ResultsPresentation::FormatType& formatType, const char* filename);

		bool remoteServer();
	
	};

}
#endif /* SIALIB_H_ */
