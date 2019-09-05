#pragma once
#include <string>

namespace simplearchive {

	class LightroomImport
	{
		std::string m_archivePath;
		std::string m_lightroomPath;
	public:
		LightroomImport(const char*archivePath, const char *lightroomPath);
		~LightroomImport();
		bool makeList();
	};

};

