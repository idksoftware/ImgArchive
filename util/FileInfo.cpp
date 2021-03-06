/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "FileInfo.h"
#include "CIDKCrc.h"
#include "md5.h"
#include <string>
#include <fstream>

#include "CLogger.h"
#include "IDKUuid.h"
#include <iostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

//#include "AppConfig.h"
#include "SAUtils.h"
namespace simplearchive {

	static std::string splitName(std::string pathStr) {
		size_t pos = pathStr.find_last_of("/");
		if (pos == std::string::npos) {
			pos = pathStr.find_last_of("\\");
			if (pos == std::string::npos) {
				return pathStr;
			}
		}
		std::string filename = pathStr.substr(pos + 1, pathStr.length() - (pos + 1));
		return filename;
	}

	static std::string getFileContents(const char *filename)
	{
		try {
			int count = 0;
			std::ifstream in(filename, std::ios::in | std::ios::binary);
			if (in)
			{
				std::string contents;
				in.seekg(0, std::ios::end);
				contents.resize((std::string::size_type)in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&contents[0], contents.size());
				in.close();
				return(contents);
			}
		}
		catch (std::exception /*&e*/) {
			throw std::exception();
		}
		std::string error;
		return error;
	}

	FileInfo::FileInfo(std::string &path)
	{
		CLogger &logger = CLogger::getLogger();

		CIDKCrc Crc;
		m_path = path;
		std::string buf;
		try {
			m_name = splitName(m_path);
			logger.log(LOG_OK, CLogger::Level::INFO, "Image: %s", m_path.c_str());
			buf = getFileContents(path.c_str());
		}
		catch (std::exception /*&e*/) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Cannot read image: %s", m_path.c_str());
			throw std::exception();
		}
		MD5 md5(buf);
		m_md5 = md5.hexdigest();
		logger.log(LOG_OK, CLogger::Level::INFO, "MD5 of image: %s is %s", path.c_str(), m_md5.c_str());

		CIDKUuid uuid;
		m_uuid = uuid.GetUuid();
		logger.log(LOG_OK, CLogger::Level::INFO, "UUID of image: %s is %s", path.c_str(), m_uuid.c_str());
		m_size = buf.length();
		logger.log(LOG_OK, CLogger::Level::INFO, "Size of image: %s is %d", path.c_str(), m_size);
		m_crc = Crc.crc((unsigned char *)buf.c_str(), m_size);
		logger.log(LOG_OK, CLogger::Level::INFO, "CRC from image: %s is %x", path.c_str(), m_crc);

		m_createTime = SAUtils::createTime(path.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "Create time of image: %s", m_createTime.toLogString().c_str());
		m_modTime = SAUtils::modTime(path.c_str());
		
	}

	FileInfo::~FileInfo()
	{
	}

	

	
}
