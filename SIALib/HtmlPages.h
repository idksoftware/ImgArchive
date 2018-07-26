#pragma once

#include <fstream>
#include <vector>
#include <memory>

class HtmlPage
{
	std::vector<std::string> m_link;
	void insertLinks(std::ofstream & htmlFile);
protected:
	virtual void head(std::ofstream &htmlFile) {};
	virtual void header(std::ofstream &htmlFile) {};
	virtual void content(std::ofstream &htmlFile) {};
	void addLink(const char *link);
public:
	HtmlPage();
	~HtmlPage();
	bool make(const char *path);
};

class BaseHtmlPage : public HtmlPage
{
	virtual void head(std::ofstream &htmlFile);
public:
	BaseHtmlPage();
	~BaseHtmlPage();
	
};

class AboutHtmlPage : public BaseHtmlPage
{
	void header(std::ofstream &htmlFile);
public:
	AboutHtmlPage();
	~AboutHtmlPage();
	void content(std::ofstream &htmlFile);
};

class HomeHtmlPage : public BaseHtmlPage
{
	void header(std::ofstream &htmlFile);
public:
	HomeHtmlPage();
	~HomeHtmlPage();
	void content(std::ofstream &htmlFile);
};

class YearFileItem {
public:
	std::string m_year;
	std::string m_firstDay;
	int m_numberOfDays;
	int m_numberOfImages;
};

class ArchiveHtmlPage : public BaseHtmlPage
{
	void header(std::ofstream &htmlFile);
	std::vector<YearFileItem> m_fileItems;
public:
	ArchiveHtmlPage();
	~ArchiveHtmlPage();
	void content(std::ofstream &htmlFile);
	void load(std::vector<YearFileItem> &fileItems);
};

class SystemHistoryFileItem {
public:
	std::string m_name;
	std::string m_start;
	std::string m_end;
	int m_numberOfItems;
};

class HistoryHtmlPage : public BaseHtmlPage
{
	std::vector<SystemHistoryFileItem> m_fileItems;
	void header(std::ofstream &htmlFile);
public:
	HistoryHtmlPage();
	~HistoryHtmlPage();
	void content(std::ofstream &htmlFile);
	void load(std::vector<SystemHistoryFileItem> &fileItems);
};

class DayNode {
public:
	std::string m_name;
	
};


/*
	2018
*/
/*
class YearHtmlPage : public BaseHtmlPage
{
	std::string m_year;
	std::vector<DayNode> m_items;
	void header(std::ofstream &htmlFile);
public:
	YearHtmlPage(const char *year);
	~YearHtmlPage();
	void content(std::ofstream &htmlFile);
	void load(std::vector<DayNode> &fileItems);
};

class ImageNode {
public:
	std::string m_name;

};
*/
class ImageFileItem {
public:
	std::string m_image;
	int imagewidth;
	int imageheight;
	ImageFileItem() {
		imagewidth = 0;
		imageheight = 0;
	}
};

class DayFileItem {
public:
	std::string m_day;
	std::vector<ImageFileItem> m_items;
	int m_numberOfImages;
};

class DaysHtmlPage : public BaseHtmlPage
{
	std::vector<DayFileItem> m_items;
	void head(std::ofstream &htmlFile);
	void header(std::ofstream &htmlFile);
	std::string m_targetDate;
	std::string m_year;
	void makeImageHtmlBlock(std::ofstream &htmlFile, std::vector<ImageFileItem> &items);
public:
	DaysHtmlPage(const char *year);
	~DaysHtmlPage();
	void setTargetDate(const char *date) {
		m_targetDate = date;
	}
	void content(std::ofstream &htmlFile);
	void load(std::vector<DayFileItem> &fileItems);
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

class JournalFileItem {
public:
	std::string m_date;
	std::string m_name;
	bool m_complete;
	int m_total;
	int m_incomplete;
	int m_imported;
	int m_duplicate;
	int m_error;
};

class JournalHtmlPage : public BaseHtmlPage
{
	std::vector<JournalFileItem> m_fileItems;
	void header(std::ofstream &htmlFile);
public:
	JournalHtmlPage();
	~JournalHtmlPage();
	void content(std::ofstream &htmlFile);
	void load(std::vector<JournalFileItem> &fileItems);
};

class MenuHtmlPage : public BaseHtmlPage
{
	void header(std::ofstream &htmlFile);
public:
	MenuHtmlPage();
	~MenuHtmlPage();
	void content(std::ofstream &htmlFile);
};
