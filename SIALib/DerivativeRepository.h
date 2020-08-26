
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

#pragma once

#include <thread>
#include <mutex>
#include "VersionIndex.h"
#include "VersionControl.h"
#include "TargetsList.h"
//#include "LogDocument.h"
#include "MasterCatalogue.h"
#include "SQLiteDB.h"
#include "IndexVisitor.h"
#include "ImagePath.h"
#include "ArchiveObject.h"
//using namespace SIADBLite;

#ifdef XXXXXXX

namespace simplearchive {

	/**
	 * This is the Derivatives Object that is used to archive the derivatives (new verions) of original images contained in the master archive.
	 */
	class DerivativeMetadataRow;
	class DerivativeIndexTable;
	class MetadataObject;
	class Version;
	class ArchivePath;

	class DerivativesObject {
		std::unique_ptr<DerivativeIndexTable> m_derivativeIndexTable;
		std::unique_ptr<RepositoryPath> m_repositoryPath;
		std::string m_workspacePath;

		std::shared_ptr<DerivativeMetadataRow> m_derivativeMetadata;
		RepositoryObject m_backup[2];
	public:
		DerivativesObject() noexcept :
			m_derivativeIndexTable(std::make_unique<DerivativeIndexTable>()),
			m_repositoryPath(std::make_unique<RepositoryPath>())
			//m_derivativeTable(std::make_unique<DerivativeTable>())
		{}
		~DerivativesObject() {}
		bool init(RepositoryPath& repositoryPath, const char* workspace);
		RepositoryPath& getRepositoryPath() {
			return *m_repositoryPath;
		}
		bool settupRelative(std::string& yyyymmddStr);
		bool addimage(const char* sourceRelPath, FileInfo& fileInfo, const char* comment, int primarySeqNumber, int derivativeSeqNumber, int version, MetadataObject& metadataObject);
		bool getimage(const std::string& pathToTargetRoot, const char* filepath, const char* comment, int version);
		//bool copy2Repos(const std::string &pathToSourceRoot, const std::string &file);
		//bool copy2Target(const std::string &pathToTargetRoot, const std::string &file);
		/// @brief Checkout
		/// @param fileath - distination folder were the images to be checked out reside.
		//bool checkout(const char *pathToTargetRoot, const char *targetRelPath, int v, const char *comment, const char *verstionPath, bool force);
		/// @brief Checkin
		/// @param fileath - distination folder were the images to be checked out reside.
		bool checkin(FileInfo& fileInfo, const char* comment);

		DerivativeIndexTable& getDerivativeIndexTable() {
			return *m_derivativeIndexTable;
		}

		//Version& getCurrentVersion() {
		//	return *m_currentVersion;
		//}

		DerivativeMetadataRow& getDerivativeMetadata() {
			return *m_derivativeMetadata;
		}
		bool isBackup1Enabled();
		bool isBackup2Enabled();
		RepositoryObject& getBackup1Object();
		RepositoryObject& getBackup2Object();
		virtual bool copyFile(const std::string& pathToSourceRoot, const std::string& yyyymmddStr, const std::string& fileName, const std::string& sequenceName);
		bool validate(const char* dbImage, const char* sourceImage = nullptr);
	};

} // namespace

#endif