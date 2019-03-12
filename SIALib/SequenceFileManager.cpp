#include "SequenceFileManager.h"
#include "SequenceNumber.h"
#include <string>
#include <vector>
#include "SAUtils.h"

ImageSequenceOption SequenceFileManager::m_option = ImageSequenceOption::NoRename;
std::string SequenceFileManager::m_masterPath;

SequenceFileManager::SequenceFileManager()
{
}


SequenceFileManager::~SequenceFileManager()
{
}


SequenceFileManager& SequenceFileManager::get() {
	static SequenceFileManager sequenceFileManager;
	return sequenceFileManager;
}

const char *SequenceFileManager::sequenceFile(const char *yearDay, long seq, const char * orginalName)
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
	return sequenceFile(y, m, d, seq, imagePath, orginalName);
	//return orginalName;
}

const char *SequenceFileManager::sequenceFile(int y, int m, int d, long seq, const std::string& imagePath, const char * orginalName)
{
	std::string ext = SAUtils::getExtention(orginalName);
	std::string name = SAUtils::getFilenameNoExt(orginalName);
	std::shared_ptr<ImageSequence> sequence;
	switch (m_option) {
	case ImageSequenceOption::NoRename:
		sequence = std::make_shared<NoRenameSequence>(name.c_str(), imagePath.c_str());
		break;
	case ImageSequenceOption::UseNumber:
		sequence = std::make_shared<NumberSequence>(y, m, d, seq, imagePath.c_str());
		break;
	case ImageSequenceOption::UseDate:
		sequence = std::make_shared<DateSequence>(y, m, d, seq, imagePath.c_str());
		break;
	case ImageSequenceOption::Unknown:
	default:
		return nullptr;
	}
	//NumberSequence numberSequence(y, m, d, seq, m_masterPath.c_str());
	m_name = sequence->toSequence();
	m_name += '.';
	m_name += ext;
	return m_name.c_str();
}