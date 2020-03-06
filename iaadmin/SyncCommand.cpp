#include <string>

#include "SyncCommand.h"

bool SyncCommand::parseOptions(const char* str) {
	std::string arg = str;
	if (arg.compare("Backup1") == 0) {
		m_syncBackup = SyncBackup::Backup1;
		return true;
	}
	if (arg.compare("Backup2") == 0) {
		m_syncBackup = SyncBackup::Backup1;
		return true;
	}
	if (arg.compare("Both") == 0) {
		m_syncBackup = SyncBackup::Both;
		return true;
	}
	return false;
}