#pragma once
#include <string>

class ImageEncodeBase32 {
	std::string m_encodedString;
	std::string m_imageName;
	int m_version;
	bool m_error;
	static bool m_includeVersion;
	public:
		ImageEncodeBase32(long index, const char* imageName, int version, const char* ext);
		ImageEncodeBase32(long index, const char* imageName, const char* ext);
		ImageEncodeBase32(const char* encodedString);

		virtual ~ImageEncodeBase32() = default;
		const char* getEncodedString() {
			return m_encodedString.c_str();
		}

		static void includeVersion(bool yes) {
			m_includeVersion;
		}
};

