#include "WriteMetadata.h"
#include "XMLWriter.h"
#include "JSONWriter.h"
#include "HTMLWriter.h"
#include "DerivativeMetadata.h"
#include "MetadataObject.h"
#include "SAUtils.h"
#include "CLogger.h"

namespace simplearchive {

	bool WriteMetadata::writeDerivativeMetadata(std::string& rootPath, std::string& versionName, DerivativeMetadataRow& metadataObject, std::string& imageName) {

		CLogger& logger = CLogger::getLogger();

		std::string toxml = rootPath + '/' + imageName;
		if (SAUtils::DirExists(toxml.c_str()) == false) {
			if (SAUtils::mkDir(toxml.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to create version folder: \"%s\"?", toxml.c_str());
				return false;
			}

		}
		return writeDerivativeMetadata(toxml, versionName, metadataObject);
	}

	bool WriteMetadata::writeMetadata2Workspace(std::string& workspace, std::string& imageAddress, MetadataObject& metadataObject) {

		PathController pathController;
		if (pathController.splitShort(imageAddress.c_str()) == false) {
			return false;
		}
		
		std::string fullPath = workspace;

		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			return false;
		}

		fullPath += '/';
		fullPath += pathController.getYear();

		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				return false;
			}
		}

		fullPath += '/';
		fullPath += pathController.getYearday();
	
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				return false;
			}
		}
		fullPath += "/.imga";
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				return false;
			}
		}
		fullPath += "/metadata";
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				return false;
			}
		}

		if (WriteMetadata::writeMasterMetadata(fullPath, pathController.getImage(), metadataObject) == false) {
			return false;
		}
		
		return true;
	}

	
	bool WriteMetadata::writeDerivativeMetadat2Workspace(std::string& workspace, std::string& imageAddress, std::string& versionName, DerivativeMetadataRow& derivativeMetadata) {
		
		PathController pathController;
		if (pathController.splitShort(imageAddress.c_str()) == false) {
			return false;
		}

		std::string fullPath = workspace;

		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			return false;
		}

		fullPath += '/';
		fullPath += pathController.getYear();

		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				return false;
			}
		}

		fullPath += '/';
		fullPath += pathController.getYearday();

		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				return false;
			}
		}
		fullPath += "/.imga";
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				return false;
			}
		}
		fullPath += "/metadata";
		if (SAUtils::DirExists(fullPath.c_str()) == false) {
			if (SAUtils::mkDir(fullPath.c_str()) == false) {
				return false;
			}
		}

		if (writeDerivativeMetadata(fullPath, versionName, derivativeMetadata) == false) {
			return false;
		}
		return true;
	}

	bool WriteMetadata::writeMasterMetadata(std::string& rootPath, std::string& imageName, MetadataObject& metadataObject) {

		std::string toxml = rootPath + '/' + imageName + ".xml";
		XMLWriter xmlWriter;
		if (xmlWriter.writeImage(metadataObject, toxml.c_str()) == false) {
			return false;
		}
		std::string tojson = rootPath + '/' + imageName + ".json";
		JSONWriter jsonWriter;
		if (jsonWriter.writeImage(metadataObject, tojson.c_str()) == false) {
			return false;
		}
		std::string tohtml = rootPath + '/' + imageName + ".html";
		HTMLWriter htmlWriter;
		if (htmlWriter.writeImage(metadataObject, tohtml.c_str()) == false) {
			return false;
		}
		return true;
	}


	bool WriteMetadata::writeDerivativeMetadata(std::string& rootPath, std::string& versionName, DerivativeMetadataRow& metadataObject) {
		std::string toxml = rootPath;
		toxml += '/';
		toxml += versionName;
		toxml += ".xml";

		XMLWriter xmlWriter;
		if (xmlWriter.writeDerivativeMetadata(metadataObject, toxml.c_str()) == false) {
			return false;
		}
		/*
		std::string tojson = rootPath + '/' + imageName + ".json";
		JSONWriter jsonWriter;
		if (jsonWriter.writeDerivativeMetadata(metadataObject, tojson.c_str()) == false) {
			return false;
		}
		std::string tohtml = rootPath + '/' + imageName + ".html";
		HTMLWriter htmlWriter;
		if (htmlWriter.writeDerivativeMetadata(metadataObject, tohtml.c_str()) == false) {
			return false;
		}
		*/
		return true;
	}

	/*
	bool WriteMetadata::writeMetadata(std::string& repositoryMetadataPath, std::string& imageAddress, MetadataObject& metadataObject) {
		std::string toxml = repositoryMetadataPath + '/' + imageAddress + ".xml";
		XMLWriter xmlWriter;
		if (xmlWriter.writeImage(metadataObject, toxml.c_str()) == false) {
			return false;
		}
		std::string tojson = repositoryMetadataPath + '/' + imageAddress + ".json";
		JSONWriter jsonWriter;
		if (jsonWriter.writeImage(metadataObject, tojson.c_str()) == false) {
			return false;
		}
		std::string tohtml = repositoryMetadataPath + '/' + imageAddress + ".html";
		HTMLWriter htmlWriter;
		if (htmlWriter.writeImage(metadataObject, tohtml.c_str()) == false) {
			return false;
		}
		return true;
	}
	*/

}; // namespace