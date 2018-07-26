#include "ExportImages.h"
#include "ArchiveVisitor.h"
#include "SAUtils.h"

namespace simplearchive {

	class ExportAction : public AVAction {
		std::string m_distPath;
		std::string m_archivePath;
		bool m_isImagesOn;
		bool m_isMetadataOn;
		int m_archiveLength;
	protected:

		virtual void onStart() {
			printf("onStart\n");
		};
		/// At the end of each directory found, this function is run.
		virtual void onEnd() {
			printf("onEnd\n");
		};
		/// On finding a file, this function is run.
		virtual void onYearFolder(const char *name) {
			printf("onYearFolder %s: \n", name);
		};
		/// On finding a file, this function is run.
		virtual void onYearEnd() {
			printf("onYearEnd\n");
		};
		/// On finding a directory, this function is run.
		virtual void onDayFolder(const char *name) {
			printf("onDayFolder %s: \n", name);
		};
		/// On finding a directory, this function is run.
		virtual void onDayEnd() {
			printf("onDayEnd\n");
		};
		/// On finding a directory, this function is run.
		virtual void onImage(const char *path, const char *name) {
			if (isImage()) {
				std::string sourcePath = path;
				std::string distPath = m_distPath;
				std::string relPath = sourcePath.substr(m_archiveLength, sourcePath.length() - m_archiveLength);
				relPath = relPath.substr(0, relPath.length() - 4); // remove "data" from path
				distPath += relPath;

				//printf("\t\tonImage %s: %s %s\n", path, name, distPath.c_str());
				if (SAUtils::makePath(m_distPath.c_str(), relPath.c_str()) == false) {
					//return false;
				}
				distPath += name;
				sourcePath += '/';
				sourcePath += name;
				if (SAUtils::copy(sourcePath.c_str(), distPath.c_str()) == false) {
					//return false;
				}
			}
		};

		virtual void onMetadata(const char *path, const char *name) {
			//printf("\t\tonMetadata %s: %s\n", path, name);
			if (isMetadata()) {
				std::string sourcePath = path;
				std::string distPath = m_distPath;
				std::string relPath = sourcePath.substr(m_archiveLength, sourcePath.length() - m_archiveLength);
				relPath = relPath.substr(0, relPath.length() - 8); // remove "data" from path
				distPath += relPath;

				//printf("\t\tonMetadata %s: %s %s\n", path, name, distPath.c_str());
				if (SAUtils::makePath(m_distPath.c_str(), relPath.c_str()) == false) {
					//return false;
				}
				distPath += name;
				sourcePath += '/';
				sourcePath += name;
				if (SAUtils::copy(sourcePath.c_str(), distPath.c_str()) == false) {
					//return false;
				}
			}
		};
		/// This function is a factory function used to create new FolderVisitor objects.
		//virtual AVAction *make() { return new CatalogAction; }
	public:


		/// Constructor
		ExportAction(const char *archivePath, const char *distPath) {
			m_archivePath = archivePath;
			m_distPath = distPath;
			m_archiveLength = m_archivePath.length();
			m_isImagesOn = true;
			m_isMetadataOn = true;
		};
		/// Distructor
		virtual ~ExportAction() {
		}

		void setDistPath(const char *path) {
			m_distPath = path;
		}
		void setArchivePath(const char *path) {
			m_archivePath = path;
		}

		bool isImage() {
			return m_isImagesOn;
		}

		bool isMetadata() {
			return m_isMetadataOn;
		}
	};



	ExportImages::ExportImages(const char *MasterPath)
	{
		m_MasterPath = MasterPath;
	}


	ExportImages::~ExportImages()
	{
	}

	bool ExportImages::process() {
		ExportAction *exportAction = new ExportAction(m_MasterPath.c_str(),"c:/temp");
		ArchiveVisitor archiveVisitor(exportAction, m_MasterPath.c_str());
		
		archiveVisitor.archive();
		return true;
	}

}