
#include "ImageEncodeBase32.h"
#include "Base32.h"

ImageEncodeBase32::ImageEncodeBase32(long index, const char* imageName, int version, const char* ext)
{
	std::string ver;
	if (version < 10) {
		ver += "0";
	}
	ver += std::to_string(version);
	
	Base32Hex base32Hex;
	m_encodedString = imageName;
	m_encodedString += "_";
	m_encodedString += base32Hex.decimal2Base32(index, 6);
	m_encodedString += "[";
	m_encodedString += ver;
	m_encodedString += "]";
	m_encodedString += ".";
	m_encodedString += ext;

	m_imageName = imageName;
	m_version = version;
}
ImageEncodeBase32::ImageEncodeBase32(const char* encodedString)
{
	m_encodedString = encodedString;
	int dotPos = m_encodedString.find_last_of(".");
	std::string ext = m_encodedString.substr(dotPos + 1);
}


