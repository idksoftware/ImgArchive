#include "ImageFileNameManager.h"
#include "SequenceNumber.h"
#include "ImageEncodeBase32.h"

#include <string>
#include <vector>
#include "SAUtils.h"

ImageFileNameOption ImageFileNameManager::m_option = ImageFileNameOption::Default;
std::string ImageFileNameManager::m_masterPath;

ImageFileNameManager::ImageFileNameManager()
{
}


ImageFileNameManager::~ImageFileNameManager()
{
}


ImageFileNameManager& ImageFileNameManager::get() {
	static ImageFileNameManager imageFileNameManager;
	return imageFileNameManager;
}

const char * ImageFileNameManager::getFileName(const char *yearDay, long seq, const char * orginalName)
{
	std::string yearDayStr = yearDay;
	int y = atoi(yearDayStr.substr(2,2).c_str());
	std::string tmp = yearDayStr.substr(5, 2).c_str();
	int m = atoi(yearDayStr.substr(5, 2).c_str());
	tmp = yearDayStr.substr(5, 2).c_str();
	int d = atoi(yearDayStr.substr(8, 2).c_str());
	tmp = yearDayStr.substr(8, 2).c_str();
	std::string imagePath = m_masterPath;
	imagePath += '/';
	imagePath += yearDayStr.substr(0, 4);
	imagePath += '/';
	imagePath += yearDayStr;
	imagePath += "/images";
	return getFileName(y, m, d, seq, imagePath, orginalName);
	//return orginalName;
}

const char * ImageFileNameManager::getFileName(int y, int m, int d, long seq, const std::string& imagePath, const char * orginalName)
{
	std::string ext = SAUtils::getExtention(orginalName);
	std::string name = SAUtils::getFilenameNoExt(orginalName);
	std::shared_ptr<ImageSequence> sequence;
	switch (m_option) {
	case ImageFileNameOption::NoRename:
		sequence = std::make_shared<NoRenameSequence>(name.c_str(), imagePath.c_str());
		break;
	case ImageFileNameOption::UseNumber:
		sequence = std::make_shared<NumberSequence>(y, m, d, seq, imagePath.c_str());
		break;
	case ImageFileNameOption::UseDate:
		sequence = std::make_shared<DateSequence>(y, m, d, seq, imagePath.c_str());
		break;
	case ImageFileNameOption::Unknown:
	case ImageFileNameOption::Default:
	default:
		{
			ImageEncodeBase32 imageEncode((long)seq, name.c_str(), 0, ext.c_str());
			m_name = imageEncode.getEncodedString();
			return m_name.c_str();
		}
	}
	//NumberSequence numberSequence(y, m, d, seq, m_masterPath.c_str());
	m_name = sequence->toSequence();
	m_name += '.';
	m_name += ext;
	return m_name.c_str();
}