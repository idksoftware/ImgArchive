#pragma once
#include <string>

namespace simplearchive {

	class ExportImages
	{
		bool m_isImagesOn;
		bool m_isMetadataOn;
		std::string m_MasterPath;
	public:
		ExportImages(const char *MasterPath);
		~ExportImages();
		bool process();
		bool isImage() {
			return m_isImagesOn;
		}

		bool isMetadata() {
			return m_isMetadataOn;
		}
	};

};
