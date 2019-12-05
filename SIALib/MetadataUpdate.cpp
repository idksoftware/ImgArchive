/*
 * MetadataUpdate.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: wzw7yn
 */
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
//#include "DatabaseInterface.h"
#include "MetadataUpdate.h"
#include "MetadataDocument.h"
#include "MetadataObject.h"
#include "CSVArgs.h"
#include "DBDefines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


namespace simplearchive {



MetadataUpdate::MetadataUpdate() {
	// TODO Auto-generated constructor stub

}

MetadataUpdate::~MetadataUpdate() {
	// TODO Auto-generated destructor stub
}

bool MetadataUpdate::process(const char *imagePath, const char *imageName, const char *cmd) {
	std::string cmdString = cmd;
	if (cmdString[0] != '[' || cmdString[cmdString.length()-1] != ']') {
		return false;
	}

	std::string tmp = imagePath;
	std::string fullImagePath = tmp.substr(0, 4);
	fullImagePath += '/';
	fullImagePath += imagePath;
	/*
	DatabaseInterface &db = DatabaseInterface::get();
	std::shared_ptr<MetadataObject> mo = db.get(imageName, imagePath);
	if (!mo.get()) {
		return false;
	}
	
	cmdString = cmdString.substr(1, cmdString.length()-2);
	CSVArgs csvArgs;
	csvArgs.process(cmdString.c_str());
	for (std::vector<std::string>::iterator i = csvArgs.begin(); i != csvArgs.end(); i++) {
		std::string &data = *i;
		//std::cout << "Arg: " << data.c_str() << '\n';
		if (command(data, *mo) == false) {
			return false;
		}
	}
	db.put(imageName, imagePath, *mo);
	*/
	return true;
}

bool MetadataUpdate::command(std::string &cmdarg, MetadataObject &mo) {

	unsigned int idx = cmdarg.find_first_of('=');
	if (idx == std::string::npos) {
		return false;
	}
	std::string cmd = cmdarg.substr(0, idx);
	std::string arg = cmdarg.substr(idx +1, cmdarg.length() - (idx + 1));
	if (arg[0] != '"' || arg[arg.length()-1] != '"') {
		return false;
	}
	arg = arg.substr(1, arg.length()-2);
	//std::cout << "Cmd: " << cmd.c_str() << " Arg: " << arg.c_str() << '\n';
	const MTTableSchema& schema = mo.getSchema();
	int colIdx = schema.getIndex(cmd.c_str());
	if (colIdx == -1) {
		return false;
	}
	MTColumn &colObj = mo.columnAt(colIdx);
	colObj.fromString(arg);

	return true;
}

std::shared_ptr<MetadataDocument> MetadataUpdate::get(const char *imagePath, const char *imageName) {
	std::string tmp = imagePath;
	std::shared_ptr<MetadataDocument> metadataDocument = nullptr;
	/*
	DatabaseInterface &db = DatabaseInterface::get();
	std::shared_ptr<MetadataObject> mo = db.get(imageName, imagePath);
	if (!mo.get()) {
		return metadataDocument;
	}
	metadataDocument.reset(new MetadataDocument);
	metadataDocument->setImageName(imageName);
	metadataDocument->setImagePath(imagePath);
	loadMetadataDocument(*metadataDocument, *mo);
	*/
	return metadataDocument;
}

bool MetadataUpdate::loadMetadataDocument(MetadataDocument &metadataDocument, MetadataObject &metadataObject) {
	const MTTableSchema& schema = metadataObject.getSchema();
	unsigned int len = schema.size();

	for (unsigned int i = 0; i < len; i++) {
		std::string line = schema.getColumnName(i);
		line += ':';
		line += metadataObject.columnAt(i).toString();
		metadataDocument.push_back(line);
	}

	return true;
}

} /* namespace simplearchive */
