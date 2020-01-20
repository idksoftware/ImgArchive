#include <memory>
#ifdef _WIN32
#include  <io.h>
#else
#include <unistd.h>
#endif
#include  <stdio.h>  
#include  <stdlib.h> 
#include <sstream>
#include <iomanip>
#include "SAUtils.h"
#include "pathcontroller.h"

//  realpath() - linux 
namespace simplearchive {


	PathController::PathController(const char *path, bool validate)
	{

		std::unique_ptr<char> absPath(new char[1024 * 4]());
		char *fullPath;
		int outSize = 1024 * 4;
#if defined _WIN64 || defined _WIN32
		fullPath = _fullpath(absPath.get(), path, outSize);
#else
		fullPath = realpath(path , absPath.get());
#endif
		if (validate) {
			int ret = doValidate(fullPath);
			if (ret) {
				m_isValid = true;
				m_fullPath = fullPath;
				m_root = fullPath;
			}
			else {
				m_isValid = false;
			}
		}
		else {
			m_fullPath = fullPath;
			m_root = fullPath;
		}
	}

	

	PathController::PathController() : m_isValid(false)
	{
	}

	PathController::~PathController()
	{
	}


	bool PathController::doValidate(const char *path) {
#ifdef WIN32
		int ret = _access(path, 0);
#else
		int ret = access(path, 0);
#endif
		if (ret == -1) {
			ErrorCode::setErrorCode(IMGA_ERROR::FILE_NOT_FOUND);
			return false;
		}
		return true;
	}

	bool PathController::isValid() {

		return m_isValid;
	}

	std::string PathController::getFullPath() {
		return m_fullPath;
	}

	bool PathController::validateYYYY(const char *str) {
		std::string dateStr = str;
		
		if (!(dateStr.length() == 4 || dateStr.find_last_of("\\/") == 4)) {
			return false;
		}
		std::string year;
		year += dateStr[0];
		year += dateStr[1];
		year += dateStr[2];
		year += dateStr[3];

		if (year.find_first_not_of("0123456789") != std::string::npos) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		return true;
	}

	bool PathController::validateYYMMDD(const char *str) {

		std::string dateStr = str;
		
		if (!(dateStr.length() == 10 || dateStr.find_last_of("\\/") == 10)) {
			return false;
		}
		if (dateStr.find_first_of("-") != 4 || dateStr.find_first_of("-", 5) != 7)  {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		std::string year;
		year += dateStr[0];
		year += dateStr[1];
		year += dateStr[2];
		year += dateStr[3];

		if (year.find_first_not_of("0123456789") != std::string::npos) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		std::string  mon;
		mon += dateStr[5];
		mon += dateStr[6];

		if (mon.find_first_not_of("0123456789") != std::string::npos) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		std::string  day;
		day += dateStr[8];
		day += dateStr[9];

		if (day.find_first_not_of("0123456789") != std::string::npos) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		
		return true;
	}


	bool PathController::validateFullYYMMDD(const char *root, const char *yymmdd) {
		if (!SAUtils::DirExists(root)) {
			return false;
		}
		if (!validateYYMMDD(yymmdd)) {
			return false;
		}

		return true;
	}

	

	std::string PathController::makeYYYY(const char *yymmdd) {
		if (!validateYYMMDD(yymmdd)) {
			std::string s;
			return s;
		}

		std::string year = yymmdd;
		m_year = year.substr(0, 4);
		return m_year;
	}

	void PathController::setRelativePath(ExifDate &date) {
		int year = date.getYear();
		int month = date.getMonth();
		int day = date.getDay();
		//printf("Time: %.4d %.2d %.2d", year, month, day);
		m_year = std::to_string(year);

		std::stringstream s;
		s << year << '-' << std::setw(2) << std::setfill('0') << month
			<< '-' << std::setw(2) << std::setfill('0') << day;
		std::string tmp = s.str();
		m_yearday = tmp;
	}

	bool PathController::createPath(const char *root, const char *yymmdd) {

		if (!validateYYMMDD(yymmdd)) {
			return false;
		}

		std::string year = yymmdd;
		m_year = year.substr(0, 4);

		std::string path = root;
		path += '/';
		path += m_year;

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
		m_fullPath = path;
		return true;
	}

	bool PathController::splitPathAndFile(const char *path) {
		std::string tmpPath = path;
		int pos = 0;
		if ((pos = tmpPath.find_first_of("0123456789")) == std::string::npos) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		if (pos == 0) { // first char a number
			int pos = tmpPath.find_last_of("\\/");
			m_relativePath = tmpPath.substr(0, pos);
			m_image = tmpPath.substr(pos + 1, tmpPath.length() - (pos + 1));
			
			m_year = m_relativePath.substr(0, 4);
			m_yearday = m_relativePath.substr(5, tmpPath.length() - 5);
			if (validateYYMMDD(m_yearday.c_str()) == false) {
				ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
				return false;
			}
		}
		else {
			m_fullPath = path;
			int pos = m_fullPath.find_last_of("\\/");
			m_root = m_fullPath.substr(0, pos);
			m_image = m_fullPath.substr(pos + 1, m_fullPath.length() - (pos + 1));
		}
		return true;
	}

	bool PathController::split() {
		int pos = m_fullPath.find_last_of("\\/");
		m_root = m_fullPath.substr(0, pos);
		m_image = m_fullPath.substr(pos + 1, m_fullPath.length() - (pos + 1));
		pos = m_root.find_last_of("\\/");
		std::string yymmdd = m_root.substr(pos + 1, m_fullPath.length() - (pos + 1));
		if (PathController::validateYYMMDD(yymmdd.c_str()) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		m_root = m_fullPath.substr(0, pos);
		pos = m_root.find_last_of("\\/");
		std::string yyyy = m_root.substr(pos + 1, m_fullPath.length() - (pos + 1));
		if (yyyy.find_first_not_of("0123456789") != std::string::npos) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		m_yearday = yymmdd;
		m_year = yyyy;
		m_relativePath = m_year + "/" + m_yearday;
		m_root = m_fullPath.substr(0, pos);

		return true;
	}
}; /* namespace simplearchive */
