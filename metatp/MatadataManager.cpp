#include "MatadataManager.h"
#include "XMLWriter.h"
#include "JSONWriter.h"
#include "HTMLWriter.h"

namespace simplearchive {

	MatadataManager::MatadataManager()
	{
	}


	MatadataManager::~MatadataManager()
	{
	}

	bool MatadataManager::writeMetadata(std::string& rootPath, std::string &imageName, MetadataObject &metadataObject) {

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

}