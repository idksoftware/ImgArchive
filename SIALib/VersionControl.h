#pragma once
#include <string>
#include <memory>
#include "ImageIndex.h"
#include "MetaType.h"
#include "VersionIndex.h"
#include "Version.h"
#include "VersionsList.h"

namespace simplearchive {

	class VersionTable;
	class MTRow;
	class VersionMetadataObject;
//	class Version;

	class VersionControl
	{
		static std::string m_pathToPrimary;
		static std::string m_pathToMaster;
		static std::string m_pathToDerivative;
		static std::string m_workspace;
		std::string m_relativeWorkspacePath;
		std::unique_ptr<VersionIndex> m_versionIndex;
		std::shared_ptr<Version> m_currentVersion;
		int m_version { 0 };
		std::string m_currentVersionPath;
		std::string m_md5;
		unsigned int m_crc { 0 };
		bool checkoutFromMaster(const char *targetRelPath, const char *comment, bool force);
		bool checkoutFromDerivatives(const char *targetRelPath, const char *comment, bool force);
		bool makeVersionsList(const char * filepath);
	public:

		VersionControl() : m_versionIndex(std::make_unique<VersionIndex>()) {};
		~VersionControl() = default;
		bool setCurrentVersion(const char *filepath);
		void setCurrentVersion(const char *repositoryPath, const char *sourceRelPath, const char *fileName, int version);


		static void setPaths(const char *primary, const char *master, const char *derivative, const char *workspace);
		void setVersionPath(const char *s) {
			m_versionIndex->setVersionPath(s);
		}
		bool checkinCurrentVersion(const char *database, const char *workspace) {
			return m_currentVersion->checkin(database, workspace);
		}
		bool checkin(const char *relWorkspace, const char *relDb, bool force);
		bool checkout(const char *relWorkspace, const char *relDb, bool force);
		bool checkoutFile(const char *filepath, const char *comment, bool force);
		bool uncheckoutFile(const char *filepath, const char *comment, bool force);
		bool getVersions(const char *filepath, const char *versions, bool force);

		std::string &getCurrentVersionPath() {
			return m_currentVersionPath;
		}

		int getVersion() {
			return m_version;
		}

		int getCRC() {
			return m_crc;
		}

		std::string getMD5() {
			return m_md5;
		}

		bool createMasterVersion(const BasicMetadata &bm, const char *path, size_t masterSeqNumber, size_t primarySeqNumber) {
			return m_versionIndex->createMasterVersion(bm, path, masterSeqNumber, primarySeqNumber);
		}
		bool createDerivativeVersion(FileInfo& fileInfo, size_t derivativeSeqNumber, size_t primarySeqNumber);

		const Version& getCurrentVersion() {
			return *m_currentVersion;
		}

		static VersionControl &getInstance();

		static std::string getRootPrimaryPath() {
			return m_pathToPrimary;
		}

		static std::string getRootMasterPath() {
			return m_pathToMaster;
		}
			
		static std::string getRootDerivativePath() {
			return m_pathToDerivative;
		}

		static std::string getRootWorkspace() {
			return m_workspace;
		}
	};

}