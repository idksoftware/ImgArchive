#pragma once
#include "ExifDate.h"

namespace simplearchive {
	class ImageContainer;
	/**
	* @brief This class defines how the Archive date found.
	* The Archive date is the date images are archived.
	*/
	class ArchiveDate
	{
		static bool m_useEXIFDate;
		static bool m_useFileDate;
		static bool m_useDate;
		static bool m_useDateToday;
		static ExifDate m_date;
		ExifDate *m_exifDate;
		ExifDate *m_fileDate;
		ExifDate m_achiveDate;
		static void setFalse();
		void processExifDate();
		void processFileDate(const char *path);
	public:
		/// Constructor
		ArchiveDate();
		/// Distructor
		~ArchiveDate();
		/// 
		static bool getUseEXIFDate();
		static void setUseEXIFDate(bool b);

		static bool getUseFileDate();
		static void setUseFileDate(bool b);

		static bool getUseDate();
		static void setUseDate(ExifDate date);

		static bool getUseDateToday();
		static void setUseDateToday(bool b);

		bool process(ImageContainer &ic);

		void setExitDate(ExifDate& d);
		void setFileDate(ExifDate& d);
		static void setDate(ExifDate& d);

		ExifDate getArchiveDate();
	};
}
