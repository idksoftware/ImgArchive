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
