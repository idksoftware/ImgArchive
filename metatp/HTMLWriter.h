#pragma once

#pragma once
#include <string>

namespace simplearchive {

	class MetadataObject;
	class IndexMetadataObject;
	class SideCar;

	class HTMLWriter {
		static std::string writeTag(const char *tag, const std::string &value);
	public:
		HTMLWriter();
		virtual ~HTMLWriter();
		bool writeImage(MetadataObject &container, const char *path);
		//bool writeGroup(SideCar &sideCar, const char *path);
		//bool writeIndexImage(IndexMetadataObject &container, const char *path);
	};

} /* namespace simplearchive */
