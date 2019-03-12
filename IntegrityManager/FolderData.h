#pragma once

#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "CSVArgs.h"

namespace simplearchive {

	class FolderFile {
	public:
		enum class Status {
			FOUND_Ok,		//< Found ok.
			NOT_FOUND,		//< Folder not found in archive.
			EXTRA_FOLDER,	//< Found a folder not included in the check data. 
			UNKNOWN			//< Unknown state.
		};
	private:
		std::string m_folderName;
		int m_nFiles;
		int m_nFolders;
		time_t m_time;
		Status m_status;
	public:
		FolderFile() {
			m_status = Status::NOT_FOUND;
			m_nFiles = 0;
			m_nFolders = 0;
			m_time = 0;
			
		};
		explicit FolderFile(std::string &data) {
			m_status = Status::NOT_FOUND;
			CSVArgs csvArgs(':');
			try {
				csvArgs.process(data.c_str());
				/*
				int delim1 = data.find_first_of(":");
				int delim2 = data.find_first_of(":", delim1+1);
				int delim3 = data.find_first_of(":", delim2+1);
				m_folderName = data.substr(0,delim1);
				std::string nFolderStr = data.substr(delim1+1, delim2-(delim1+1));
				m_nFolders = strtol(nFolderStr.c_str(),NULL,10);
				std::string nFilesStr = data.substr(delim2+1, delim3-(delim2+1));
				m_nFiles = strtol(nFilesStr.c_str(),NULL,10);
				std::string timeStr = data.substr(delim3+1, data.length());
				m_time = strtol(timeStr.c_str(),NULL,10);
				*/
				m_folderName = csvArgs.at(0);
				std::string nFolderStr = csvArgs.at(1);
				m_nFolders = strtol(nFolderStr.c_str(), NULL, 10);
				std::string nFilesStr = csvArgs.at(2);
				m_nFiles = strtol(nFilesStr.c_str(), NULL, 10);
				std::string timeStr = csvArgs.at(3);
				m_time = strtol(timeStr.c_str(), NULL, 10);
			}
			catch (std::exception/* &e */) {
				printf("Data corupt");
			}
		};

		FolderFile(const char *folderName, int folders = 0, int files = 0) {
			m_status = Status::NOT_FOUND;
			m_time = 0;
			time(&m_time);
			m_folderName = folderName;
			m_nFiles = files;
			m_nFolders = folders;
		}

		virtual ~FolderFile() {
		}

		std::string toString() {
			std::stringstream s;
			s << m_folderName << ':' << m_nFolders << ':' << m_nFiles << ':' << m_time;
			return s.str();
		}

		const std::string& getFolderName() const {
			return m_folderName;
		}

		std::string getYear() const {
			return m_folderName.substr(0, 4);
		}

		void setFolderName(const std::string& folderName) {
			m_folderName = folderName;
		}

		time_t getTime() const {
			return m_time;
		}

		void setTime(time_t time) {
			m_time = time;
		}

		bool match(const char *filename) {
			if (m_folderName.compare(filename) == 0) {
				return true;
			}
			return false;
		}

		int getNFiles() const {
			return m_nFiles;
		}

		void setNFiles(int nFiles) {
			m_nFiles = nFiles;
		}

		void incNFiles() {
			m_nFiles++;
		}

		int getNFolders() const {
			return m_nFolders;
		}

		void setNFolders(int nFolders) {
			m_nFolders = nFolders;
		}

		void incNFolders() {
			m_nFolders++;
		}
	};

	/*
	This container is used to write the check data out and check the validity of the data using the status field
	*/
	class FileDataContainer : public std::vector<FolderFile> {
	public:


		FileDataContainer() {};
		virtual ~FileDataContainer() {};

		bool read(const char *datafile);
		bool write(const char *datafile);
		//bool add(const char *name, int crc, const char *md5);
		int find(const char *datafile);
		bool add(const char *filename);
		bool incFiles(const char *filename);
		bool incFolders(const char *filename);
	};

} // Namespace
