#include "FolderData.h"
namespace simplearchive {

bool FileDataContainer::read(const char *datafile) {
	std::string text;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (std::getline(file, text)) {
		push_back(FolderFile(text));
	}
	file.close();

	return true;
}

bool FileDataContainer::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile &data = *i;
		file << data.toString() << '\n';
		//printf("%s\n", data.toString().c_str());
	}
	file.close();
	return true;
}

int FileDataContainer::find(const char *filename) {

	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile &data = *i;
		if (data.match(filename) == true) {
			return true;
		}

	}
	return false;
}

bool FileDataContainer::add(const char *filename) {
	FolderFile folderFile(filename);

	int pos = 0;
	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile data = *i;
		//printf("%s", data.c_str());
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			return false;
		}
		if (extStr.compare(filename) >= 0) {
			insert(begin() + (pos), folderFile);
			return true;
		}
		pos++;
	}
	push_back(folderFile);
	return false;

}

bool FileDataContainer::incFolders(const char *filename) {
	FolderFile folderFile(filename);

	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFolders();
			return true;
		}
	}
	return false;
}

bool FileDataContainer::incFiles(const char *filename) {
	FolderFile folderFile(filename);

	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFiles();
			return true;
		}
	}
	return false;
}
/*
void FileDataContainer::add(bool isYear, const char *filename) {
FolderFile folderFile(isYear, filename);
int pos = find(isYear, filename);
insert(begin()+(pos), folderFile);
}
*/
} // Namespace
