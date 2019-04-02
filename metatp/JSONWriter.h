#pragma once
#include <string>

namespace simplearchive {

	class MetadataObject;
	class IndexMetadataObject;
	class SideCar;

	class JSONWriter {
		static std::string writeTag(const char *tag, const std::string &value, bool end = false);
	public:
		JSONWriter();
		virtual ~JSONWriter();
		bool writeImage(MetadataObject &container, const char *path);
		//bool writeGroup(SideCar &sideCar, const char *path);
		//bool writeIndexImage(IndexMetadataObject &container, const char *path);
	};

} /* namespace simplearchive */
