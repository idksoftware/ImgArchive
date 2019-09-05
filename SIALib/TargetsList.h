/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#pragma once
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "DirectoryVisitor.h"
#include "ImportJournal.h"

namespace simplearchive {

	class ImageItem {
		std::string m_path;
	public:
		ImageItem(const char *path) : m_path(path)
		{}
		virtual ~ImageItem() {
			//printf("deleting item %s\n", m_path.c_str());
		}
		void print() {
			//printf("Hook Item %s\n", m_path.c_str());
		}

		void processFileHook();

		void processHook();

		const char *getPath() {
			return m_path.c_str();
		}
		// Linux  dirname() and basename()
		std::string getFilename() {
			unsigned int pos = m_path.find_last_of("/");
			if (pos == std::string::npos) {
				pos = m_path.find_last_of("\\");
			}
			std::string filename = m_path.substr(pos+1, m_path.length() - (pos+1));
			return filename;
		}
	};

	class ImageSet : public std::vector<std::shared_ptr<ImageItem>> {
		std::string m_path;
	public:
		ImageSet(const char *path) {
			m_path = path;
		}
		virtual ~ImageSet() {
			clear();
			//for (std::vector<std::shared_ptr<ImageItem>>::iterator i = this->begin(); i != this->end(); i++) {
			//	std::shared_ptr<ImageItem>data = *i;
			//	delete data;
			//}
			//printf("deleting item %s\n", m_path.c_str());
		}
		void print() {
			//printf("Set %s\n", m_path.c_str());
			for (auto i = this->begin(); i != this->end(); i++) {
				std::shared_ptr<ImageItem> data = *i;
				data->print();
			}
		}
		void processHook() {
			
			//printf("Hook process %s\n", m_path.c_str());
			for (auto i = this->begin(); i != this->end(); i++) {
				std::shared_ptr<ImageItem> data = *i;
				
				data->processHook();
			}
		}

		void processImportJournal(ImportJournal& importJournal) {
			for (auto i = this->begin(); i != this->end(); i++) {
				std::shared_ptr<ImageItem>data = *i;
				importJournal.add(data->getPath());
				
			}
		}

		const char *getPath() {
			return m_path.c_str();
		}
	};
	class ImageSets : public std::vector <std::shared_ptr<ImageSet>> {
	public:
		virtual ~ImageSets() {
			clear();
			//for (auto i = this->begin(); i != this->end(); i++) {
			//	std::shared_ptr<ImageSet> data = *i;
			//	delete data;
			//}
			//printf("deleting item %s\n", m_path.c_str());
		}
		void print() {

			for (auto i = this->begin(); i != this->end(); i++) {
				std::shared_ptr<ImageSet> data = *i;
				data->print();
			}
		}
		std::shared_ptr<ImageSet> find(const char *folder) {

			for (auto i = this->begin(); i != this->end(); i++) {
				std::shared_ptr<ImageSet> data = *i;
				if (strcmp(data->getPath(), folder) == 0) {
					return data;
				}
			}
			return nullptr;
		}
		void processHook() {

			for (auto i = this->begin(); i != this->end(); i++) {
				std::shared_ptr<ImageSet> data = *i;
				data->processHook();
			}
		}
		void processImportJournal(ImportJournal& importJournal) {
			for (auto i = this->begin(); i != this->end(); i++) {
				std::shared_ptr<ImageSet> data = *i;
				data->processImportJournal(importJournal);
			}
		}

	};


	class ImportImageList {
		static int m_folderCount;
		static int m_fileCount;
		static std::shared_ptr<ImageSets> m_imageSets;
		std::shared_ptr<ImageSet> m_imageSet;
		
	public:
		bool onStart(const char *path) {
			if (m_imageSets == nullptr) {
				m_imageSets = std::make_shared<ImageSets>();
			}
			m_imageSet = std::make_shared<ImageSet>(path);
			m_imageSets->insert(m_imageSets->end(), m_imageSet);
			return true;
		};

		bool onFile(const char *path);

		bool onDirectory(const char *path) {
			m_folderCount++;
			return true;
		};

		bool onEnd() {
			return true;
		};

		ImportImageList() {};
		virtual ~ImportImageList() {};
		static std::shared_ptr<ImageSets> getImageSets() {
			return m_imageSets;
		}
		static void destroy() {
			m_imageSets = nullptr;
		}

		static int getFileCount() {
			return m_fileCount;
		}

		static int getFolderCount() {
			return m_folderCount;
		}
	};


/// @class TargetsList TargetsList.h "TargetsList.h"
/// @brief This class Provides the target list. this contains the target images.
/// @note this is the raw list. Further processing is required to process the list into
/// true image sets.
class TargetsList
{
	static int m_fileCount;
	static int m_folderCount;
public:
	TargetsList();
	virtual ~TargetsList();

	/// This function processes all the folders under the root folder and creates
	/// a number of Images sets, one per folder found.
	void process(const char *path);

	/// This returns the resulting Image Sets 
	
	static std::shared_ptr<ImageSets> getImageSets();
	static void destroy();
	static int getFileCount();
	static int getFolderCount();

};

} /* namespace simplearchive */
