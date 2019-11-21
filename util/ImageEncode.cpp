#include <sstream>
#include "time.h"
#include "ImageEncode.h"
#include "Base64.h"

ImageEncode::ImageEncode(const char *shortRelPath, const char *imageName, int version, const char *ext) {

	m_shortRelPath = shortRelPath;

	Base64 base64;
	std::string date = m_shortRelPath;

	if (date[4] != '-' && date[7] != 7 && 10) {
		return;
	}
	std::string ys = date.substr(0, 4);
	int y = std::stoi(ys);
	std::string ms = date.substr(5, 2);
	int m = std::stoi(ms);
	std::string ds = date.substr(8, 2);
	int d = std::stoi(ds);

	int days = days_from_civil(y, m, d);

	m_encodedString = base64.decimal2base64(days);
	m_encodedString += "@";
	m_encodedString += imageName;
	m_encodedString += "@";
	m_encodedString += base64.decimal2base64(version);
	m_encodedString += ".";
	m_encodedString += ext;

	m_imageName = imageName;
	m_version = version;
	m_error = true;
}

ImageEncode::ImageEncode(const char *encodedString) {
	//m_shortRelPath = shortRelPath;
	//m_imageName = imageName;
	//m_version = version;
	m_encodedString = encodedString;
	std::string ds = m_encodedString.substr(0, 3);
	Base64 base64;
	int days = base64.base64ToDecimal(ds);
	const time_t secs = days * IDK_DATE_SECONDS_IN_DAY;
	tm* now = localtime(&secs);
	tm today = { 0 };
	int year = now->tm_year + 1900;
	int mon = now->tm_mon + 1;
	int mday = now->tm_mday;
	std::stringstream s;
	s << year << '-' << mon << '-' << mday;
	m_shortRelPath = s.str();
	int pos = m_encodedString.find_first_of('@', 4);
	m_imageName = m_encodedString.substr(4, pos - 4);
	std::string vs = m_encodedString.substr(pos + 1, 1);

	m_version = base64.base64ToDecimal(vs);
	m_error = false;
}

// Returns number of days since civil 1970-01-01.  Negative values indicate
//    days prior to 1970-01-01.
// Preconditions:  y-m-d represents a date in the civil (Gregorian) calendar
//                 m is in [1, 12]
//                 d is in [1, last_day_of_month(y, m)]
//                 y is "approximately" in
//                   [numeric_limits<Int>::min()/366, numeric_limits<Int>::max()/366]
//                 Exact range of validity is:
//                 [civil_from_days(numeric_limits<Int>::min()),
//                  civil_from_days(numeric_limits<Int>::max()-719468)]

int ImageEncode::days_from_civil(int y, unsigned m, unsigned d)
{

	y -= m <= 2;
	const int era = (y >= 0 ? y : y - 399) / 400;
	const unsigned yoe = y - era * 400;      // [0, 399]
	const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;  // [0, 365]
	const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;         // [0, 146096]
	return era * 146097 + doe - 719468;
}

