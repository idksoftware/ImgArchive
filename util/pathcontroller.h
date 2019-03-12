#pragma once

#include <string>
#include "ErrorCode.h"
#include "ExifDate.h"


namespace simplearchive {

	class PathController
	{
		std::string m_fullPath;
		std::string m_relativePath;
		bool m_isValid;

		std::string m_year;
		std::string m_yearday;
		std::string m_image;
		std::string m_root;
		
	public:
		PathController(const char *path, bool valid = true);
		PathController();
		~PathController();

		std::string getFullPath() const {
			return m_fullPath;
		}

		void setFullPath(std::string &fullPath) {
			m_fullPath = fullPath;
		}

		std::string getImage() const {
			return m_image;
		}

		void setImage(std::string &image) {
			m_image = image;
			m_fullPath += '/';
			m_fullPath += m_image;
		}

		bool isIsValid() const {
			return m_isValid;
		}

		std::string getRelativePath() const {
			return m_relativePath;
		}

		std::string getShortRelativePath() const {
			return m_relativePath;
		}

		void setRelativePath(ExifDate &exifDate);

		void setRelativePath(const char *path) {
			std::string relativePath = path;
			setRelativePath(relativePath);
		}

		void setRelativePath(std::string &relativePath) {
			m_relativePath = relativePath;
		}

		std::string getRoot() const {
			return m_root;
		}

		void setRoot(std::string &root) {
			m_root = root;
		}

		std::string getYear() const {
			return m_year;
		}

		void setYear(std::string &year) {
			m_year = year;
		}

		std::string getYearday() const {
			return m_yearday;
		}

		std::string getImageName() const {
			return m_image;
		}

		void setYearday(std::string yearday) {
			m_yearday = yearday;
		}

		bool split();
		/**
		split from form <yyyy>/<yy>-<mm>-<dd>/<imagename.ext>
		*/
		bool split(const char *img) {
			std::string imagePath = img;
			size_t pos = imagePath.find('-');
			if (pos != 4) {
				pos = imagePath.find("\\/");
				return splitLong(img);
			}
			return splitShort(img);
		}

		bool splitLong(const char *img) {
			std::string imagePath = img;

			m_year = imagePath.substr(0, 4);
			m_yearday = imagePath.substr(5, 10);
			m_image = imagePath.substr(16, imagePath.length() - 16);
			return true;
		}

		/**
		split from form <yyyy>-<mm>-<dd>/<imagename.ext>
		*/
		bool splitShort(const char *img) {
			std::string imagePath = img;
			m_year = imagePath.substr(0, 4);
			m_yearday = imagePath.substr(0, 10);
			if (imagePath.length() <= 10)  {
				// no image
				ErrorCode::setErrorCode(SIA_ERROR::NO_IMAGE);
				return false;
			}
			m_image = imagePath.substr(11, imagePath.length() - 9);
			return true;
		}

		bool setRoot(const char *rootPath) {
			if (doValidate(rootPath)) {
				m_root = rootPath;
				return true;
			}
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			return false;
		}

		/**
		This creates a full image path in the form <root>/<year>/<year-month-day>/<image.ext>.
		from the path components.
		*/
		bool makeImagePath(const char *ext = nullptr) {

			std::string path = m_root;
			if (m_year.empty() == true) {
				if (m_relativePath.empty()) {
					return false;
				}
				if (split(m_relativePath.c_str()) == false) {
					return false;
				}
			}
			std::string pathr = m_year;
			pathr += '/'; pathr += m_yearday;
			pathr += '/'; pathr += m_image;
			path += '/'; path += pathr;
			m_relativePath = pathr;
			if (ext != nullptr) {
				path += "."; path += ext;
			}
			if (doValidate(path.c_str())) {
				m_isValid = true;
			}
			else {
				m_isValid = false;
				ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			}
			m_fullPath = path;
			return m_isValid;
		}

		
		/**
		This creates a full image path in the form <root>/<year>/<year-month-day>.
		from the path components.
		*/
		bool makePath(bool validate = true) {

			std::string path = m_root;
			path += '/'; path += m_year;
			path += '/'; path += m_yearday;
			std::string imagepath = path;
			imagepath += '/'; imagepath += m_image;
			if (validate) {
				if (doValidate(imagepath.c_str())) {
					m_isValid = true;
				}
				else {
					m_isValid = false;
					ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
				}
			} 
			m_fullPath = path;
			return m_isValid;
		}


		/**
		This creates a Relative image path in the form <year>/<year-month-day>/<image.ext>.
		from the path <year-month-day>/<image.ext>.
		*/
		bool makeRelativePath(const char *p) {
			splitShort(p);

			std::string path = "/"; path += m_year;
			path += '/'; path += m_yearday;
			m_relativePath = path;
			return true;
		}

		bool makeRelativeImagePath(const char *p) {
			splitShort(p);

			std::string path = "/"; path += m_year;
			path += '/'; path += m_yearday;
			path += '/'; path += m_image;
			m_relativePath += path;
			return true;
		}
		/**
		Validate relative path using the path passed
		*/
		bool validateRelative(const char *path) {
			std::string tmp = path;
			tmp += '/';
			tmp += m_relativePath;
			return true;
		}

		/**
		 * Validate full path 
		 */
		bool ValidateFull() {
			return doValidate(m_fullPath.c_str());
		}

		bool splitPathAndFile(const char *path);
		/**
		Does a path validate on the path passed
		*/
		static bool doValidate(const char *path);

		/**
		gets the full path built from a make path function
		*/
		std::string getFullPath();

		/**
		Returns of the currently build path is valid.
		*/
		bool isValid();

		/**
		make year (YYYY) string from YY.
		For exmaple 17-01-23 converts to 2017
		*/
		std::string makeYYYY(const char *yymmdd);

		static bool validateYYYY(const char *str);
		/**
		Validates if the short path YY-MM-DD is valid.
		*/
		static bool validateYYMMDD(const char *str);

		static bool validateFullYYMMDD(const char *root, const char *yymmdd);
		/**
		creates a path using the path root and the short path YY-MM-DD.
		This will expand to <root>/<yyyy>/<yy-mm-dd>.
		Note this is successfull will set the full path.
		*/
		bool createPath(const char *root, const char *yymmdd);

		static bool has_only_digits(const std::string s){
			return s.find_first_not_of("0123456789") == std::string::npos;
		}
	};

}; /* simplearchive */