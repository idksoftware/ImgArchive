#pragma once

#include <string>

namespace simplearchive {

	class FileOperations {
	public:
		FileOperations() = delete;
		~FileOperations() = delete;

		bool isEqual(const char* source, const char* target);
	};

	class SyncArchive {
		std::string m_sourcePath;
		std::string m_targetPath;
		std::string m_sourceCkdskRoot;
		std::string m_destinationCkdskRoot;
		std::string m_curDate;
		bool process();
		bool process(std::string &dateString);
	public:
		SyncArchive(const char* source, const char* target);
		~SyncArchive();
		bool sync();
		
	};

	

	
};

