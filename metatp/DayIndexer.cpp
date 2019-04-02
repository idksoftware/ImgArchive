#include "DayIndexer.h"
#include "SAUtils.h"
#include "pathcontroller.h"
/*
	path in the form yy-mm-dd
*/

namespace simplearchive {

	bool DayIndexer::validate(const char *root, const char *yymmdd) {
		if (!PathController::validateFullYYMMDD(root, yymmdd)) {
			return false;
		}
		
		return true;
	}

	bool DayIndexer::createPath(const char *root, const char *yymmdd) {
		PathController pathController;
		if (!pathController.createPath(root, yymmdd)) {
			return false;
		}
		return true;
	}

/*
bool has_only_digits(const std::string s){
	return s.find_first_not_of("0123456789") == std::string::npos;
}


bool validateYYMMDD(const char *str) {

	std::string dateStr = str;
	if (dateStr.length() != 8 || dateStr.find_first_of("-") != 3 || dateStr.find_last_of("-") != 6)  {
		return false;
	}
	std::string year;
	year += dateStr[0];
	year += dateStr[1];

	if (year.find_first_not_of("0123456789") == std::string::npos) return false;

	std::string  mon;
	mon += dateStr[4];
	mon += dateStr[5];

	if (mon.find_first_not_of("0123456789") == std::string::npos) return false;

	std::string  day;
	day += dateStr[7];
	day += dateStr[8];

	if (mon.find_first_not_of("0123456789") == std::string::npos) return false;

	return true;
}


namespace simplearchive {

	DayIndexer::DayIndexer()
	{
	}


	DayIndexer::~DayIndexer()
	{
	}


	bool validate(const char *root, const char *yymmdd) {
		if (!SAUtils::DirExists(root)) {
			return false;
		}
		if (!validateYYMMDD(yymmdd)) {
			return false;
		}
		return true;
	}
	bool createPath(const char *root, const char *yymmdd) {

		if (!validateYYMMDD(yymmdd)) {
			return false;
		}

		std::string year;
		year += yymmdd[0];
		year += yymmdd[1];


		int y = std::stoi(year);
		if (y > 70) {
			y += 1900;
		}
		else {
			y + 2000;
		}
		year = std::to_string(y);
		std::string path = root;
		path += '/';
		path += year;

		if (!SAUtils::DirExists(path.c_str())) {
			if (!SAUtils::mkDir(path.c_str())) {
				return false;
			}
		}
		path += '/';
		path += yymmdd;
		if (!SAUtils::DirExists(path.c_str())) {
			if (!SAUtils::mkDir(path.c_str())) {
				return false;
			}
		}
		return true;
	}
	*/
} // namespace simplearchive