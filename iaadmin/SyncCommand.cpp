#include <string>

#include "SyncCommand.h"
#include "SIALib.h"
#include "AdminAppOptions.h"
#include "SAUtils.h"

namespace simplearchive {

	bool SyncCommand::setArchive(const char* str)
	{
		AppOptions& appOptions = AppOptions::get();

		std::string arg = str;
		if (arg.compare("master") == 0) {
			m_archive = SIALib::MainArchives::Master;
			
			return true;
		}
		if (arg.compare("derivative") == 0) {
			m_archive = SIALib::MainArchives::Derivative;
			return true;
		}
		if (arg.compare("Both") == 0) {
			m_archive = SIALib::MainArchives::Both;
			return true;
		}
		return false;
	}

	bool SyncCommand::setBackup(const char* str) {
		std::string arg = str;
		if (SAUtils::isEquals(str, "Backup1")) {
			m_syncBackup = SIALib::Backups::Backup_1;
			return true;
		}
		if (SAUtils::isEquals(str, "Backup2")) {
			m_syncBackup = SIALib::Backups::Backup_1;
			return true;
		}
		if (SAUtils::isEquals(str, "Both")) {
			m_syncBackup = SIALib::Backups::Both;
			return true;
		}
		return false;
	}

	SIALib::MainArchives SyncCommand::getArchive()
	{
		return m_archive;
	}

	SIALib::Backups SyncCommand::getBackup()
	{
		return m_syncBackup;
	}

};