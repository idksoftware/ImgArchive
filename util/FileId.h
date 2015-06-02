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

#pragma once

#include <string>
#include <cstdlib>
#include "ExifDateTime.h"

namespace simplearchive {

	class FileId
	{
		std::string m_name;
		std::string m_path;
		unsigned long m_crc;
		std::string m_mediaType;
		std::string m_md5;
		std::string m_uuid;
		unsigned long m_size;
		ExifDateTime m_modTime;
		ExifDateTime m_createTime;
		
	public:
		FileId(std::string &path);
		~FileId();

		unsigned long getCrc() const {
			return m_crc;
		}

		void setCrc(unsigned long crc) {
			this->m_crc = crc;
		}

		unsigned long getSize() const {
			return m_size;
		}

		void setSize(unsigned long crc) {
			this->m_size = crc;
		}

		const std::string& getMd5() const {
			return m_md5;
		}

		void setMd5(const std::string& md5) {
			this->m_md5 = md5;
		}

		const std::string& getMediaType() const {
			return m_mediaType;
		}

		void setMediaType(const std::string& mediaType) {
			this->m_mediaType = mediaType;
		}

		const std::string& getUuid() const {
			return m_uuid;
		}

		void setUuid(const std::string& uuid) {
			this->m_uuid = uuid;
		}

		const std::string& getName() const {
			return m_name;
		}

		void setName(const std::string& name) {
			this->m_name = name;
		}

		const std::string& getPath() const {
			return m_path;
		}

		void setPath(const std::string& path) {
			this->m_path = path;
		}

		const std::string getExt() const {
			return m_path.substr(m_path.find_last_of(".") + 1);
		}

		const ExifDateTime &getCreateTime() const {
			return m_createTime;
		}

		const ExifDateTime &getModTime() const {
			return m_modTime;
		}

	};

}