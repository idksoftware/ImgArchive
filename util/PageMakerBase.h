#pragma once
#include <string>
#include <memory>
#include <vector>



typedef std::vector<std::string> DaysProcessed;

class DayNode {
public:
	std::string m_name;

};

/*
	This is the information needed for an image
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

class DayPage {
public:
	std::string m_day;
	std::string m_filePath;
	std::vector<std::shared_ptr<ImageFileItem>> m_items;
	int m_numberOfImages;
	DayPage(const char* day) {
		m_numberOfImages = 0;
	}
	/*
	DayPage(DayPage &d) {
		m_day = d.m_day;
		m_numberOfImages = d.m_numberOfImages;

	}
	*/
};



/*
		This is the information needed for a full day Item
	*/
class DayFolderItem {
public:
	std::string m_day;
	std::vector<ImageFileItem> m_totalItems;
	std::vector<DayPage> m_pageItems;
	size_t m_numberOfImages;
};
