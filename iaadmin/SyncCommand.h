#pragma once
class SyncCommand
{
public:
	enum class SyncBackup {
		Backup1,
		Backup2,
		Both
	};
private:
	SyncBackup m_syncBackup;
public:
	SyncCommand() = default;
	~SyncCommand() = default;

	bool parseOptions(const char* str);
};

