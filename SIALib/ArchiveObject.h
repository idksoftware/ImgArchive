#pragma once
#include <thread>
#include <mutex>
#include "VersionIndex.h"
//#include "ImageGroup.h"
#include "TargetsList.h"
#include "LogDocument.h"
#include "MasterCatalogue.h"
#include "SQLiteDB.h"
//using namespace SIADBLite;

namespace simplearchive {

	class PrimaryIndexTable;
	class PrimaryIndex;
	class ArchiveDate;
	
	class RepositoryPath;
	class PrimaryIndexPath;
	class SIADBLiteDatabase;

	class PrimaryIndexObject {
		std::unique_ptr<PrimaryIndexTable> m_primaryIndexTable;
		std::unique_ptr<VersionIndex> m_versionIndex;
		std::unique_ptr<ImageIndex> m_imageIndex;
		std::unique_ptr<PrimaryIndexPath> m_primaryIndexPath;
		
		
		//std::unique_ptr<SIADBLite::SIADBLiteDatabase> m_DBLiteDatabase;

	public:
		PrimaryIndexObject() noexcept :
			m_primaryIndexTable(std::make_unique<PrimaryIndexTable>()),
			m_versionIndex(std::make_unique<VersionIndex>()),
			m_imageIndex(std::make_unique<ImageIndex>()),
			m_primaryIndexPath(std::make_unique<PrimaryIndexPath>())
			
		{}
		~PrimaryIndexObject() {}

		PrimaryIndexObject(PrimaryIndexObject&) = delete;
		void operator=(PrimaryIndexObject const&) = delete;

		bool init(PrimaryIndexPath &primaryIndexPath);
			
		PrimaryIndexPath &getPrimaryIndexPath() {
			return *m_primaryIndexPath;
		}
		ImageIndex &getimageIndex() {
			return *m_imageIndex;
		}
		bool addMasterImage(const BasicMetadata &BasicMetadata, ImagePath &imagePath, int masterSeqNumber, int primarySeqNumber);
		bool addDerivativeImage(FileInfo& fileInfo, const char *comment, int primarySeqNumber, int derivativeSeqNumber, int version);

		PrimaryIndexTable &getPrimaryIndexTable() {
			return *m_primaryIndexTable;
		}

		VersionIndex &getVersionIndex() {
			return *m_versionIndex;
		}

		
//		DerivativeTable &getDerivativeRow();
	};

	/**
	 * This is the Derivatives Object that is used to archive the derivatives (new verions) of original images contained in the master archive.  
	 */
	class DerivativeMetadata;
	class DerivativeIndexTable;
	class MetadataObject;

	class DerivativesObject {
		std::unique_ptr<DerivativeIndexTable> m_derivativeIndexTable;
		std::unique_ptr<RepositoryPath> m_repositoryPath;
		std::string m_workspacePath;
	public:
		DerivativesObject() noexcept :
			m_derivativeIndexTable(std::make_unique<DerivativeIndexTable>()),
			m_repositoryPath(std::make_unique<RepositoryPath>())
			//m_derivativeTable(std::make_unique<DerivativeTable>())
		{}
		~DerivativesObject() {}
		void init(RepositoryPath &repositoryPath, const char *workspace);
		RepositoryPath &getRepositoryPath() {
			return *m_repositoryPath;
		}
		bool settupRelative(std::string &yyyymmddStr);
		bool addimage(FileInfo& fileInfo, const char *comment, int primarySeqNumber, int derivativeSeqNumber, int version, MetadataObject& metadataObject);
		bool getimage(const std::string &pathToTargetRoot, const char *filepath, const char *comment, int version);
		bool copy2Repos(const std::string &pathToSourceRoot, const std::string &file);
		bool copy2Target(const std::string &pathToTargetRoot, const std::string &file);
		/// @brief Show checked out files into the Workspace
		/// @param fileath - distination folder were the images that may be checked out.
		bool showCheckedOut(const char *filepath);
		/// @brief Show un-checked out changes in the Workspace
		/// @param fileath - distination folder were the images that may be checked out.
		bool showUncheckedOutChanges(const char *filepath);
		/// @brief Checkout
		/// @param fileath - distination folder were the images to be checked out reside.
		bool checkout(const char *pathToTargetRoot, const char *targetRelPath, int v, const char *comment, const char *verstionPath, bool force);
		/// @brief Checkin
		/// @param fileath - distination folder were the images to be checked out reside.
		bool checkin(FileInfo &fileInfo, const char *comment);
		/// @brief Checkin
		/// @param fileath - distination folder were the images to be checked out reside.
		bool uncheckout(const char *filepath, const char *comment);

		DerivativeIndexTable &getDerivativeIndexTable() {
			return *m_derivativeIndexTable;
		}
		
	};

	class MasterIndexTable;	
	
	class RepositoryObject {
		std::unique_ptr<MasterIndexTable> m_masterIndexTable;
		//std::unique_ptr<ImageIndex> m_imageIndex;
		std::unique_ptr<RepositoryPath> m_repositoryPath;
	public:
		RepositoryObject() noexcept :
			m_masterIndexTable(std::make_unique<MasterIndexTable>()),
			//m_imageIndex(std::make_unique<ImageIndex>()),
			m_repositoryPath(std::make_unique<RepositoryPath>())
		{}
		~RepositoryObject() {}
		void init(RepositoryPath &repositoryPath);
		RepositoryPath &getRepositoryPath() {
			return *m_repositoryPath;
		}
		bool settupRelative(std::string &yyyymmddStr);
		bool writeMetadata(ImagePath &imagePath, MetadataObject &metadataObject);
		bool copyFile(const std::string  &pathToSourceRoot, const std::string &file);
		//bool CreateImage(const BasicMetadata &BasicMetadata, ImagePath &imagePath, CSVDBFile &csvDBFile, MetadataObject &metadataObject);
		MasterIndexTable &getMasterIndexTable() {
			return *m_masterIndexTable;
		}
		/// @brief Checkout
		/// @param fileath - distination folder were the images to be checked out reside.
		bool checkout(const char *pathToTargetRoot, const char *targetRelPath, const char *comment, bool force);
	};

	class MasterCatalogue;
	class ArchiveObject
	{
		//PrimaryIndex *m_primaryIndex;
		//ArchiveDate *m_archiveDate;
		std::string m_masterPath;
		std::string m_workspacePath;

		RepositoryObject m_master;
		RepositoryObject m_backup[2];
		DerivativesObject m_derivatives;
		std::unique_ptr<MasterCatalogue> m_masterView;
		std::unique_ptr<PrimaryIndexObject> m_PrimaryIndexObject;
		
		bool isWorkspaceEnabled();
		bool isMasterEnabled();
		bool isBackup1Enabled();
		bool isBackup2Enabled();
		
		static std::unique_ptr<ArchiveObject> m_this;
		static std::once_flag m_onceFlag;
		bool TestForDuplicate(FileInfo& fileinfo, ImportJournal& importJournal, bool isForceChanges);

		ArchiveObject(const ArchiveObject& src) {};
		ArchiveObject& operator=(const ArchiveObject& rhs) { return *m_this; };	
		
		ArchiveObject() noexcept;
	public:
		
		~ArchiveObject();
		//static ArchiveObject& getInstance();
		static ArchiveObject& getInstance()
		{
			static ArchiveObject    instance; // Guaranteed to be destroyed.
								  // Instantiated on first use.
			return instance;
		}

		//bool Initalise(const char *pathToWorkspace, const char *pathToMaster, const char *pathToHome, const char *pathToHistory);
		bool Initalise();
		bool OnCompletion();
		bool CreateImage(const BasicMetadata &BasicMetadata, ImagePath &imagePath, MetadataObject &metadataObject);
		bool processHistory(ImagePath &imagePath, const char *comment);
		//bool processHistory(ImagePath &imagePath, const char *filepath, const char *comment, const HistoryEvent &he, int ver);
		bool processImageGroupSets(ImageSets *imageSets, ImportJournal& importJournal);
		/// @brief Show checked out files into the Workspace
		/// @param fileath - distination folder were the images that may be checked out.
		bool showCheckedOut(const char *filepath);
		/// @brief Show un-checked out changes in the Workspace
		/// @param fileath - distination folder were the images that may be checked out.
		bool showUncheckedOutChanges(const char *filepath);
		/// @brief Checkout
		/// @param fileath - distination folder were the images to be checked out reside.
		bool checkout(const char *scope, const char *comment, bool force);
		/// @brief Checkin
		/// @param fileath - distination folder were the images to be checked out reside.
		bool checkoutFile(const char *filepath, const char *comment, bool force);
		bool checkin(const char *scope, const char *comment, bool force);
		bool checkinFile(const char *filepath, const char *comment, bool force);
		bool uncheckout(const char *scope, const char *comment, bool force);
		bool uncheckoutFile(const char *filepath, const char *comment, bool force);
		/// @brief Checkin
		/// @param fileath - distination folder were the images to be checked out reside.
		bool get(const char *filepath, const char *comment, bool force);
		bool exportImages(const char *dispPath);
		bool listContents(const char *addressScope);

		bool writeMetadata(std::string rootPath, ImagePath &imagePath, MetadataObject &metadataObject);
		//PrimaryIndex &getPrimaryIndex() {
		//	return *m_primaryIndex;
		//}

		//ArchiveDate &getArchiveDate() {
		//	return *m_archiveDate;
		//}
		bool settupRelative(std::string &yyyymmddStr);
		bool writeMetadata2Workspace(ImagePath &imagePath, MetadataObject &metadataObject);
		bool writeMetadata2PrimaryIndex(ImagePath &imagePath, MetadataObject &metadataObject);
		bool writeMetadata2MasterDatabase(ImagePath &imagePath, MetadataObject &metadataObject);
		bool writeMetadata2DerivativesDatabase(ImagePath &imagePath, MetadataObject &metadataObject);
		bool copyFile(const std::string  &pathToSourceRoot,const std::string &file);

		const RepositoryPath &getMasterPath() {
			return getMasterObject().getRepositoryPath();
		}
		const std::string getWorkspacePath() {
			return m_workspacePath;
		}
		PrimaryIndexObject& getPrimaryIndexObject();
		DerivativesObject& getDerivativesObject();
		RepositoryObject& getMasterObject();
		RepositoryObject& getBackup1Object();
		RepositoryObject& getBackup2Object();
		MasterCatalogue& ArchiveObject::getMasterCatalogue();

		bool imageHistory(const char *filepath, const LogDocument::FormatType& formatType);
	};

} // simplearchive