#pragma once
#include <string>
/// This class defines the file type (picture, raw or 
class ImageType {
public:
	enum Type {
		UNKNOWN_EXT = -1,
		PICTURE_EXT,
		RAW_EXT,
	};
private:
	Type m_type;
public:
	ImageType() {
		m_type = UNKNOWN_EXT;
	}
	ImageType(Type type) {
		m_type = type;
	}
	std::string toString() {
		switch (m_type) {
		case UNKNOWN_EXT:
			return "Unknown";
		case PICTURE_EXT:
			return "Picture";
		case RAW_EXT:
			return "RAW";
		}
		return "Unknown";
	}
	bool fromString(const char *str) {
		std::string s = str;
		if (s.compare("Unknown") == 0) {
			m_type = UNKNOWN_EXT;
			return true;
		} else
		if (s.compare("Picture") == 0) {
			m_type = PICTURE_EXT;
			return true;
		} else
		if (s.compare("Raw") == 0) {
			m_type = RAW_EXT;
			return true;
		} else {
			m_type = UNKNOWN_EXT;
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
