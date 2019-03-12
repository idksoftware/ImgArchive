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
#include "ExifDate.h"
#include <memory>

namespace simplearchive {
	
	class ImageContainer;
	/**
	* @brief This class defines how the Archive date found.
	* The Archive date is the date images are archived.
	*/
	class ArchiveDate
	{
	public: 
		enum UsingDate {
			USING_FORCED_DATE,
			USING_FORCED_DATE_TODAY,
			USING_FORCED_FILE_DATE,
			USING_DATE_TODAY,
			USING_DEFAULT_DATE,
			USING_FILE_DATE,
			USING_EXIF_DATE,
			USING_CAPTURE_RAW_DATE,
			USING_CAPTURE_PIC_DATE,
			USING_FILE_RAW_DATE,
			USING_FILE_PIC_DATE,
			UNKNOWN
		};
	private:
		static bool m_useEXIFDate;
		static bool m_useFileDate;
		static bool m_useThisDate;
		static bool m_useDateToday;
		static bool m_forceDate;
		static ExifDate m_date;
		static bool m_defaultDateSet;
		std::shared_ptr<ExifDate> m_exifDate;
		std::shared_ptr<ExifDate> m_fileDate;
		
		ExifDate m_achiveDate;
		static void setFalse();
		void processExifDate();
		void processFileDate(const char *path);
		UsingDate m_usingDate;
	public:
		/// Constructor
		ArchiveDate();
		/// Distructor
		~ArchiveDate();
		// 
		static bool getForceDate();
		static void setForceDate(bool b);

		static bool getUseEXIFDate();
		static void setUseEXIFDate(bool b);

		static bool getUseFileDate();
		static void setUseFileDate(bool b);

		static bool getUseDate();
		static void setUseDate(const ExifDate &date);

		static bool getUseDateToday();
		static void setUseDateToday(bool b);

		static bool getDefaultDateSet();
		static void setDefaultDateSet(bool b);


		bool process(ImageContainer &ic);

		void setExifDate(const ExifDate& d);
		void setFileDate(const ExifDate& d);
		static void setDate(const ExifDate& d);

		ExifDate getArchiveDate();

		std::string getUsingDateString();
	};
}
