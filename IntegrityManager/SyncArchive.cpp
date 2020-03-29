
#include "SAUtils.h"
#include "CIDKCrc.h"
#include "SyncArchive.h"

namespace simplearchive {

	class ProcessImages {
		std::string m_sourceDateRoot;
		std::string m_destinationDateRoot;
		std::string m_dateString;
	public:
		ProcessImages(std::string &sourceDateRoot, std::string &destinationDateRoot, std::string& dateString);
		~ProcessImages();
		bool sync();
	};

	ProcessImages::ProcessImages(std::string& sourceDateRoot, std::string& destinationDateRoot, std::string& dateString)
		: m_sourceDateRoot(sourceDateRoot),
		m_destinationDateRoot(destinationDateRoot),
		m_dateString(dateString)
		{};

	ProcessImages::~ProcessImages() {};

	bool ProcessImages::sync() {
		
		std::string year = m_dateString.substr(0, 4);
		std::string curSourceDateRoot = m_sourceDateRoot;
		curSourceDateRoot += '/'; curSourceDateRoot += year;
		curSourceDateRoot += '/'; curSourceDateRoot += m_dateString;

		std::string sourceDateRootImages = curSourceDateRoot;
		sourceDateRootImages += '/'; sourceDateRootImages += "images";

		std::string curdestinationDateRoot = m_destinationDateRoot;
		curdestinationDateRoot += '/'; curdestinationDateRoot += year;
		if (SAUtils::DirExists(curdestinationDateRoot.c_str()) == false) {
			if (SAUtils::mkDir(curdestinationDateRoot.c_str()) == false) {
				throw std::exception(); //"checkdisk path not found");
			}
		}
		curdestinationDateRoot += '/'; curdestinationDateRoot += m_dateString;
		if (SAUtils::DirExists(curdestinationDateRoot.c_str()) == false) {
			if (SAUtils::mkDir(curdestinationDateRoot.c_str()) == false) {
				throw std::exception(); //"checkdisk path not found");
			}
		}

		std::string destinationRootImages = curdestinationDateRoot;
		destinationRootImages += '/'; destinationRootImages += "images";
		if (SAUtils::DirExists(destinationRootImages.c_str()) == false) {
			if (SAUtils::mkDir(destinationRootImages.c_str()) == false) {
				throw std::exception(); //"checkdisk path not found");
			}
		}

		std::string sourceRootMetadataList = curSourceDateRoot;
		sourceRootMetadataList += '/'; sourceRootMetadataList += "metadata";
		
		std::string destinationRootMetadataList = curdestinationDateRoot;
		destinationRootMetadataList += '/'; destinationRootMetadataList += "metadata";
		if (SAUtils::DirExists(destinationRootMetadataList.c_str()) == false) {
			if (SAUtils::mkDir(destinationRootMetadataList.c_str()) == false) {
				throw std::exception(); //"checkdisk path not found");
			}
		}

		FileList_Ptr filelist = SAUtils::getFiles_(sourceDateRootImages.c_str());
		for (auto i = filelist->begin(); i != filelist->end(); i++) {
			std::string imageString = *i;
			if (imageString[0] == '.') {
				continue;
			}
			printf("Image: %s\n", imageString.c_str());
			std::string sourceDateRootImage = sourceDateRootImages;
			sourceDateRootImage += '/'; sourceDateRootImage += imageString;

			std::string destinationRootImage = destinationRootImages;
			destinationRootImage += '/'; destinationRootImage += imageString;

			if (SAUtils::FileExists(destinationRootImage.c_str()) == false) {
				printf("%s does not exist \n", imageString.c_str());
				printf("  copy %s\n   to %s\n", sourceDateRootImage.c_str(), destinationRootImage.c_str());
				if (SAUtils::copy(sourceDateRootImage.c_str(), destinationRootImage.c_str()) == false) {
					return false;
				}
				std::string sourceRootMetadata = sourceRootMetadataList;
				sourceRootMetadata += '/'; sourceRootMetadata += imageString;

				std::string destinationRootMetadata = destinationRootMetadataList;
				destinationRootMetadata += '/'; destinationRootMetadata += imageString;
				// html
				std::string sourceRootMetadatahtml = sourceRootMetadata + ".html";
				std::string destinationRootMetadatahtml = destinationRootMetadata + ".html";
				//printf("  copy %s\n   to %s\n", sourceRootMetadatahtml.c_str(), destinationRootMetadatahtml.c_str());
				if (SAUtils::copy(sourceRootMetadatahtml.c_str(), destinationRootMetadatahtml.c_str()) == false) {
					return false;
				}
				// json
				std::string sourceRootMetadataJson = sourceRootMetadata + ".json";
				std::string destinationRootMetadataJson = destinationRootMetadata + ".json";
				//printf("  copy %s\n   to %s\n", sourceRootMetadataJson.c_str(), destinationRootMetadataJson.c_str());
				if (SAUtils::copy(sourceRootMetadataJson.c_str(), destinationRootMetadataJson.c_str()) == false) {
					return false;
				}
				// xml
				std::string sourceRootMetadataXml = sourceRootMetadata + ".xml";
				std::string destinationRootMetadataXml = destinationRootMetadata + ".xml";
				//printf("  copy %s\n   to %s\n", sourceRootMetadataXml.c_str(), destinationRootMetadataXml.c_str());
				if (SAUtils::copy(sourceRootMetadataXml.c_str(), destinationRootMetadataXml.c_str()) == false) {
					return false;
				}
			}
		}
		return true;
	}


	bool FileOperations::isEqual(const char* source, const char* target)
	{
		std::string sourceBuf;
		int sourceSize = SAUtils::getFileContents(source, sourceBuf);

		std::string targetBuf;
		int targetSize = SAUtils::getFileContents(target, targetBuf);

		if (sourceSize != targetSize) {
			return false;
		}
		CIDKCrc Crc;

		unsigned long sourceCrc = Crc.crc((unsigned char*)sourceBuf.c_str(), sourceSize);
		unsigned long targetCrc = Crc.crc((unsigned char*)targetBuf.c_str(), targetSize);

		if (sourceCrc != targetCrc) {
			return false;
		}
		return false;
	}

	SyncArchive::SyncArchive(const char* source, const char* target) : m_sourcePath(source), m_targetPath(target) {};
	SyncArchive::~SyncArchive() {};

	bool SyncArchive::sync()
	{
		m_sourceCkdskRoot = m_sourcePath;
		m_sourceCkdskRoot += "/";
		m_sourceCkdskRoot += "system";
		m_sourceCkdskRoot += "/";
		m_sourceCkdskRoot += "chdsk";
		if (SAUtils::DirExists(m_sourceCkdskRoot.c_str()) == false) {
			throw std::exception(); //"checkdisk path not found");
		}

		m_destinationCkdskRoot = m_targetPath;
		m_destinationCkdskRoot += "/";
		m_destinationCkdskRoot += "system";
		if (SAUtils::DirExists(m_destinationCkdskRoot.c_str()) == false) {
			if (SAUtils::mkDir(m_destinationCkdskRoot.c_str()) == false) {
				throw std::exception(); //"checkdisk path not found");
			}
		}
		m_destinationCkdskRoot += "/";
		m_destinationCkdskRoot += "chdsk";
		if (SAUtils::DirExists(m_destinationCkdskRoot.c_str()) == false) {
			if (SAUtils::mkDir(m_destinationCkdskRoot.c_str()) == false) {
				throw std::exception(); //"checkdisk path not found");
			}
		}
		bool res = process();
		return res;
	}

	bool SyncArchive::process() {


		std::string sourceCkdskPath = m_sourcePath;
		sourceCkdskPath += "/system/chdsk";
		FileList_Ptr filelist = SAUtils::getFiles_(sourceCkdskPath.c_str());
		for (auto i = filelist->begin(); i != filelist->end(); i++) {
			std::string dataString = *i;
			if (dataString[0] == '.') {
				continue;
			}

			std::string year = dataString.substr(0, 4);

			if (year.compare("fdata.csv") == 0) {
				continue;
			}
			if (year.compare("fdata.xml") == 0) {
				continue;
			}
			//printf("Year found %s\n", year.c_str());
			//validateReportingObject.startYear(year.c_str());
			std::string yearPath = sourceCkdskPath + '/';
			yearPath += year;

			FileList_Ptr yearlist = SAUtils::getFiles_(yearPath.c_str());
			for (auto i = yearlist->begin(); i != yearlist->end(); i++) {
				std::string dateString = *i;
				if (dateString[0] == '.') {
					continue;
				}

				printf("month %s\n", dateString.c_str());
				std::string fullDateString = yearPath;
				if (process(dateString) == false) {
					return false;
				}

			}

			std::string sourceCkdskPathXml = sourceCkdskPath;
			sourceCkdskPathXml += '/'; sourceCkdskPathXml += "fdata.xml";

			std::string destinationCkdskRootXml = m_destinationCkdskRoot;
			destinationCkdskRootXml += '/'; destinationCkdskRootXml += "fdata.xml";

			if (SAUtils::copy(sourceCkdskPathXml.c_str(), destinationCkdskRootXml.c_str()) == false) {
				return false;
			}
		}
		return true;
	}

	bool SyncArchive::process(std::string &dateString) {
		std::string year = dateString.substr(0, 4);
		std::string curSourceDateRoot = m_sourceCkdskRoot;
		curSourceDateRoot += '/'; curSourceDateRoot += year;
		curSourceDateRoot += '/'; curSourceDateRoot += dateString;

		std::string curDestinationCkdskRoot = m_destinationCkdskRoot;
		curDestinationCkdskRoot += '/'; curDestinationCkdskRoot += year;
		if (SAUtils::DirExists(curDestinationCkdskRoot.c_str()) == false) {
			if (SAUtils::mkDir(curDestinationCkdskRoot.c_str()) == false) {
				throw std::exception(); //"checkdisk path not found");
			}
		}

		curDestinationCkdskRoot += '/'; curDestinationCkdskRoot += dateString;
		if (SAUtils::DirExists(curDestinationCkdskRoot.c_str()) == false) {
			if (SAUtils::mkDir(curDestinationCkdskRoot.c_str()) == false) {
				throw std::exception(); //"checkdisk path not found");
			}
		}

		std::string curDestinationCkdskFile = curDestinationCkdskRoot;
		curDestinationCkdskFile += '/'; curDestinationCkdskFile += "fdata.csv";
		if (SAUtils::FileExists(curDestinationCkdskFile.c_str()) == false) {

			std::string sourceDateRoot = m_sourcePath;
			sourceDateRoot += '/'; sourceDateRoot += year;
			sourceDateRoot += '/'; sourceDateRoot += dateString;

			std::string destinationDateRoot = m_targetPath;
			destinationDateRoot += '/'; destinationDateRoot += year;
			destinationDateRoot += '/'; destinationDateRoot += dateString;


			ProcessImages processImages(m_sourcePath, m_targetPath, dateString);
			if (processImages.sync() == false) {
				return false;
			}
			std::string curSourceCkdskFile = curSourceDateRoot;
			curSourceCkdskFile += '/'; curSourceCkdskFile += "fdata.csv";
			//printf("  copy %s\n   to %s\n", curSourceCkdskFile.c_str(), curDestinationCkdskFile.c_str());
			if (SAUtils::copy(curSourceCkdskFile.c_str(), curDestinationCkdskFile.c_str()) == false) {
				return false;
			}
			std::string curSourceCkdskXml = curSourceDateRoot;
			curSourceCkdskXml += '/'; curSourceCkdskXml += "fdata.xml";

			std::string curDestinationCkdskXml = curDestinationCkdskRoot;
			curDestinationCkdskXml += '/'; curDestinationCkdskXml += "fdata.xml";
			//printf("  copy %s\n   to %s\n", curSourceCkdskXml.c_str(), curDestinationCkdskXml.c_str());
			if (SAUtils::copy(curSourceCkdskXml.c_str(), curDestinationCkdskXml.c_str()) == false) {
				return false;
			}
		}
		return true;
	}
};