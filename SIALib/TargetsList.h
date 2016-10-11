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
#include "DirectoryVisitor.h"
#include "ImportJournal.h"

namespace simplearchive {

	class ImageItem {
		std::string m_path;
	public:
		ImageItem(const char *path) {
			m_path = path;

		}
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

	class ImageSet : public std::vector < ImageItem * > {
		std::string m_path;
	public:
		ImageSet(const char *path) {
			m_path = path;
		}
		virtual ~ImageSet() {
			for (std::vector<ImageItem *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageItem *data = *i;
				delete data;
			}
			//printf("deleting item %s\n", m_path.c_str());
		}
		void print() {
			//printf("Set %s\n", m_path.c_str());
			for (std::vector<ImageItem *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageItem *data = *i;
				data->print();
			}
		}
		void processHook() {
			
			//printf("Hook process %s\n", m_path.c_str());
			for (std::vector<ImageItem *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageItem *data = *i;
				
				data->processHook();
			}
		}

		void processImportJournal(ImportJournal& importJournal) {
			for (std::vector<ImageItem *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageItem *data = *i;
				importJournal.add(data->getPath());
				
			}
		}

		const char *getPath() {
			return m_path.c_str();
		}
	};
	class ImageSets : public std::vector < ImageSet * > {
	public:
		virtual ~ImageSets() {
			for (std::vector<ImageSet *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageSet *data = *i;
				delete data;
			}
			//printf("deleting item %s\n", m_path.c_str());
		}
		void print() {

			for (std::vector<ImageSet *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageSet *data = *i;
				data->print();
			}
		}
		ImageSet *find(const char *folder) {

			for (std::vector<ImageSet *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageSet *data = *i;
				if (strcmp(data->getPath(), folder) == 0) {
					return data;
				}
			}
			return nullptr;
		}
		void processHook() {

			for (std::vector<ImageSet *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageSet *data = *i;
				data->processHook();
			}
		}
		void processImportJournal(ImportJournal& importJournal) {
			for (std::vector<ImageSet *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageSet *data = *i;
				data->processImportJournal(importJournal);
			}
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
	static ImageSets *getImageSets();
	static void destroy();
	static int getFileCount();
	static int getFolderCount();

};

} /* namespace simplearchive */
