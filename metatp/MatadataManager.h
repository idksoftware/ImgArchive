#pragma once
#include <string>
namespace simplearchive {
	class MetadataObject;
	class MatadataManager
	{
	public:
		MatadataManager();
		~MatadataManager();
		bool writeMetadata(std::string& rootPath, std::string &imageName, MetadataObject &metadataObject);
		//bool updateMetadata(std::string& rootPath, std::string &imageName, MetadataObject &metadataObject, );
	};
};
