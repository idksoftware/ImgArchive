#pragma once


#include <string>
#include "DerivativeMetadata.h"
#include "MetadataObject.h"
#include "pathcontroller.h"
//class MetadataObject;
//class DerivativeMetadataRow;

namespace simplearchive {

	

	class WriteMetadata
	{
		static bool writeDerivativeMetadata(std::string& rootPath, std::string& versionName, DerivativeMetadataRow& metadataObject);
	public:

		static bool writeDerivativeMetadata(std::string& rootPath, std::string& versionName, DerivativeMetadataRow& metadataObject, std::string& imageName);
		
		static bool writeMasterMetadata(std::string& rootPath, std::string& imageName, MetadataObject& metadataObject);

		static bool writeMetadata2Workspace(std::string& workspace, std::string& imageAddress, MetadataObject& metadataObject);
		static bool writeDerivativeMetadat2Workspace(std::string& workspace, std::string& imageAddress, std::string& versionName, DerivativeMetadataRow& derivativeMetadata);

		//static bool writeMetadata(std::string& repositoryMetadataPath, std::string& imageAddress, MetadataObject& metadataObject);
	};

}; // namespace
