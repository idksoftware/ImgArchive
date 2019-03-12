#pragma once
#include <string>
enum class ImageSequenceOption {
	Unknown,
	UseDate,
	UseNumber,
	NoRename,
};

class SequenceFileManager
{
	std::string m_name;
	static std::string m_masterPath;	
		// Note: The sequence manager will only rename master image
		// names.
	static ImageSequenceOption m_option;
	const char *SequenceFileManager::sequenceFile(int y, int m, int d, long seq, const std::string& imagePath, const char * orginalName);
public:
	SequenceFileManager();
	~SequenceFileManager();
	static void setMasterPath(const char *path) {
		m_masterPath = path;
	}
	static void setOption(ImageSequenceOption option) {
		m_option = option;
	}
	const char *SequenceFileManager::sequenceFile(const char *yearDay, long dec, const char * orginalName);
	
	static SequenceFileManager& get();
};

