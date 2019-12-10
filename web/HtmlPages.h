#pragma once

#include <fstream>
#include <vector>
#include <memory>
#include "BaseHtmlPage.h"
#include "PageMakerBase.h"

namespace simplearchive {

	
	
	

	class DaysHtmlPage : public BaseHtmlPage
	{
		std::vector<DayFolderItem> m_dayFolderItem;
		void head(std::ofstream &htmlFile);
		void header(std::ofstream &htmlFile);
		std::string m_targetDate;
		std::string m_year;
		void makeImageHtmlBlock(std::ofstream &htmlFile, std::vector<ImageFileItem> &items);
		void makeImageHtmlBlock(std::ofstream &htmlFile, std::vector<std::shared_ptr<ImageFileItem>> &items);
		size_t m_currentPage;
	public:
		DaysHtmlPage(const char *year);
		~DaysHtmlPage();
		void setTargetDate(const char *date) {
			m_targetDate = date;
		}
		void makeSet(const char *path);
		void content(std::ofstream &htmlFile);
		void load(std::vector<DayFolderItem> &fileItems);
	};

	class ImageDatails {
	public:
		std::string image;
		int imagewidth;
		int imageheight;
		// file
		std::string sequenceID;	// *
		std::string filename;
		std::string orginalName;
		std::string filePath;
		std::string uUID;
		std::string mediaType;
		std::string md5;
		std::string crc;
		std::string fileSize;
		std::string dateCreate;
		std::string dateModified;
		std::string dateAdded;
		// Description
		std::string description;
		std::string label;
		std::string title;
		std::string subject;
		std::string rating;
		std::string ratingPercent;
		std::string tags;
		std::string keywords;
		std::string version;
		// Origin
		std::string comment;
		std::string author;
		std::string captureDate;
		std::string subSetimeOriginal;
		std::string copyright;
		// Image
		std::string width;
		std::string height;
		std::string viewRotation;
		std::string bitsPersample;
		std::string xResolution;
		std::string yResolution;
		std::string resolutionUnit;
		std::string compression;
		// Camera
		std::string maker;
		std::string model;
		std::string exposureTime;
		std::string aperture;
		std::string isoSpeedRating;
		std::string exposureBias;
		std::string exposureProgram;
		std::string subjectDistance;
		std::string focalLength;
		std::string focalLengthIn35mm;
		std::string flash;
		std::string meteringMode;
		std::string digitalZoom;
		// Advanced photo
		std::string lightSource;
		std::string colorSpace;
		std::string whiteBalance;
		std::string sceneCaptureType;
		std::string contrast;
		std::string saturation;
		std::string sharpness;
		std::string brightnessValue;
		std::string subjectDistanceRange;
		std::string maxAperture;
		std::string compressedBitsPerPixel;
		std::string flashEnergy;
		std::string exifVersion;
		std::string software;
		std::string photometricInterpretation;
		std::string gpsTimeStamp;
		//	add(MTSchema(MTSchema::Text, DB_SENSINGMETHOD));
		// Location
		std::string latitude;
		std::string longitude;
		std::string location;
		std::string scene;
		// IPTC
		std::string sourceUrl;
		std::string usageRights;
		std::string copyrightUrl;
		std::string headline;
		std::string category;
		std::string source;
		std::string instructions;
		std::string creator;
		std::string jobTitle;
		std::string address;
		std::string city;
		std::string state;
		std::string postalCode;
		std::string country;
		std::string phone;
		std::string email;
		std::string website;

		ImageDatails() {
			imagewidth = 0;
			imageheight = 0;
		}
	};

	class ImageDetailsPage : public BaseHtmlPage
	{
		ImageDatails *m_details;
		void header(std::ofstream &htmlFile);
		void head(std::ofstream &htmlFile);
	public:
		ImageDetailsPage();
		~ImageDetailsPage();
		void content(std::ofstream &htmlFile);
		void load(ImageDatails *details);
	};

	

} // namespace simplearchive
