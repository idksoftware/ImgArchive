#pragma once
#include "SIALib.h"

namespace simplearchive {

	class SyncCommand
	{
		SIALib::Backups m_syncBackup;
		SIALib::MainArchives m_archive;
	public:
		SyncCommand() = default;
		~SyncCommand() = default;

		bool setArchive(const char* str);
		bool setBackup(const char* str);

		SIALib::MainArchives getArchive();
		SIALib::Backups getBackup();
	};

};

