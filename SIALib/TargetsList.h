#pragma once
#include <iostream>
#include <stdio.h>
#include "DirectoryVisitor.h"

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
		const char *getPath() {
			return m_path.c_str();
		}
	};
	class ImageSets : public std::vector < ImageSet * > {
	public:
		void print() {

			for (std::vector<ImageSet *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageSet *data = *i;
				data->print();
			}
		}

		void processHook() {

			for (std::vector<ImageSet *>::iterator i = this->begin(); i != this->end(); i++) {
				ImageSet *data = *i;
				data->processHook();
			}
		}
	};


/// This class Provides the target list. this contains the target images.
/// Note this is the raw list. Further processing is required to process the list into
/// true image sets.
class TargetsList
{

public:
	TargetsList();
	virtual ~TargetsList();

	/// This function processes all the folders under the root folder and creates
	/// a number of Images sets, one per folder found.
	void process(const char *path);

	/// This returns the resulting Image Sets 
	static ImageSets *getImageSets();
	static void destroy();

};

} /* namespace simplearchive */
