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
#include "SAUtils.h"

/// This class defines the file type (picture, raw or 
class ImageType {
public:
	enum class Type {
		UNKNOWN_EXT = -1,
		PICTURE_EXT,
		RAW_EXT,
	};
private:
	Type m_type;
public:
	ImageType() {
		m_type = Type::UNKNOWN_EXT;
	}
	ImageType(Type type) {
		m_type = type;
	}
	std::string toString() {
		switch (m_type) {
		case Type::UNKNOWN_EXT:
			return "Unknown";
		case Type::PICTURE_EXT:
			return "Picture";
		case Type::RAW_EXT:
			return "RAW";
		}
		return "Unknown";
	}
	bool fromString(const char *str) {
		std::string s = str;
		
		if (SAUtils::isEquals(s, "unknown")) {
			m_type = Type::UNKNOWN_EXT;
			return true;
		} else
		if (SAUtils::isEquals(s, "picture")) {
			m_type = Type::PICTURE_EXT;
			return true;
		} else
		if (SAUtils::isEquals(s, "raw")) {
			m_type = Type::RAW_EXT;
			return true;
		} else {
			m_type = Type::UNKNOWN_EXT;
		}
		return false;
	}

	Type getType() const {
		return m_type;
	}

	void setType(Type type) {
		m_type = type;
	}

	ImageType operator=(const char *str) {
		fromString(str);
		return *this;
	}
};
