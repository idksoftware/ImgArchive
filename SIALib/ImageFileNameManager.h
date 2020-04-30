#pragma once
#include <string>
enum class ImageFileNameOption {
	Unknown,
	UseDate,
	UseNumber,
	NoRename,
	Default
};

class ImageFileNameManager
{
	std::string m_name;
	static std::string m_masterPath;	
		// Note: The sequence manager will only rename master image
		// names.
	static ImageFileNameOption m_option;
	const char * getFileName(int y, int m, int d, long seq, const std::string& imagePath, const char * orginalName);
public:
	ImageFileNameManager();
	~ImageFileNameManager();
	static void setMasterPath(const char *path) {
		m_masterPath = path;
	}
	static void setOption(ImageFileNameOption option) {
		m_option = option;
	}
	const char * getFileName(const char *yearDay, long dec, const char * orginalName);
	
	static ImageFileNameManager& get();
};

