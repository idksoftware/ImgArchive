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
		PathController(const char *path, bool valid = false);
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

		bool split(const char* img);

		/**
			split from form /<yyyy/<yyyy>-<mm>-<dd>/<imagename.ext>
		*/
		bool splitLong(const char* img);

		/**
			split from form <yyyy>-<mm>-<dd>/<imagename.ext>
		*/
		bool splitShort(const char* img);

		bool setRoot(const char* rootPath);

		/**
			This creates a full image path in the form <root>/<year>/<year-month-day>/<image.ext>.
			from the path components.
		*/
		bool makeImagePath(const char* ext = nullptr);

		/**
			This creates a full image path in the form <root>/<year>/<year-month-day>.
			from the path components.
		*/
		bool makePath(bool validate = true);

		/**
			This creates a Relative image path in the form <year>/<year-month-day>/<image.ext>.
			from the path <year-month-day>/<image.ext>.
		*/
		bool makeRelativePath(const char* p);

		bool makeRelativeImagePath(const char* p);

		/**
		 * Validate full path 
		 */
		bool ValidateFull() {
			return doValidate(m_fullPath.c_str());
		}

		bool validateRelative(const char* path);

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