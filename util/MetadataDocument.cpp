/*
 * MetadataDocument.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: wzw7yn
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include "MetadataDocument.h"
#include "CSVArgs.h"

namespace simplearchive {



bool MetadataDocument::write() {
	for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
		std::cout << *i << '\n';
	}
	return true;
}

bool MetadataDocument::writeXML() {
	std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			  << "<Metadata path=\"" << m_imagePath << "\" image=\"" << m_imageName << "\">\n";

	for (std::list<std::string>::iterator i = begin(); i != end(); i++) {

		CSVArgs csvArgs(':');
		if (csvArgs.process(i->c_str()) == false) {
			return false;
		}

		std::cout << "\t<Item>\n";
		std::cout << writeTag("Label", csvArgs.at(0), 2);
		std::cout << writeTag("Value", csvArgs.at(1), 2);
		std::cout << "\t</Item>\n";

	}
	std::cout <<	"</Catalog>\n";
	return true;
}

bool MetadataDocument::writeHtml(const char *filePath) {

	std::ofstream file(filePath);
    if (!file.is_open()) {
    	return false;
    }
    file << "<table>\n";

	for (std::list<std::string>::iterator i = begin(); i != end(); i++) {

		CSVArgs csvArgs(':');
		if (csvArgs.process(i->c_str()) == false) {
			return false;
		}
		if (csvArgs.at(1).compare("null") != 0) {
			file << "\t<tr>\n";
			file << "<th>" << csvArgs.at(0) << "</th>\n";
			file << "<th>" << csvArgs.at(1) << "</th>\n";
			file << "\t</tr>\n";
		}
	}
	file <<	"</table>\n";
	file.close();
	return true;
}

} /* namespace simplearchive */
