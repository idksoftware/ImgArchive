#pragma once

#include <string>

#define IDK_DATE_SECONDS_IN_DAY				86400
/*
This class encodes an image give its short date i.e. 2017-02-10 and its version.
the encoding is found from the number of days since 1970 -01-01. The number of days are then
conerted into a base64 number. the version is also conerted into a base64 number.
The format becomes <ddd>@<image nae@<v>.<ext>
n is the three base64 digits and v is the version number also converted into a base64 number.
This reseaion for doing this is that deritive images need this encodeing for two reasions, the first is so derivative images
can be traced back to there source image and the second is to mke then unique.

All images in the derivative database will be encoded in this way. However the user way not see this encodeing as it it deficalt to read.
If the user needs to see these images thay will be copied into a folder in the workspace under the original images folder but with the foldet
name the same as the original image. This folder can contain allthe versions of the orginal image. The format of thse versions wll follow this form:

<orginal image name>[version number].<ext>
*/
class ImageEncode {
	std::string m_encodedString;
	std::string m_shortRelPath;
	std::string m_imageName;
	int m_version;
	bool m_error;
	int days_from_civil(int y, unsigned m, unsigned d);
public:
	ImageEncode(const char *shortRelPath, const char *imageName, int version, const char *ext);
	ImageEncode(const char *encodedString);
	virtual ~ImageEncode() = default;
	const char *getEncodedString() {
		return m_encodedString.c_str();
	}
};
