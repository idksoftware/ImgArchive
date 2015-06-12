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

#include "ArchiveDate.h"
#include "ImageContainer.h"
#include "MetadataObject.h"
#include "ExifDate.h"
#include "BasicExifFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	bool ArchiveDate::m_useEXIFDate = true;
	bool ArchiveDate::m_useFileDate = false;
	bool ArchiveDate::m_useDate = false;
	bool ArchiveDate::m_useDateToday = false;
	ExifDate ArchiveDate::m_date;

	ArchiveDate::ArchiveDate()
	{
		m_exifDate.reset(0);
		m_fileDate.reset(0);
	}


	ArchiveDate::~ArchiveDate()
	{
	}

	void ArchiveDate::setExitDate(ExifDate& d) {
		m_exifDate.reset(new ExifDate(d));
	}

	void ArchiveDate::setFileDate(ExifDate& d) {
		m_fileDate.reset(new ExifDate(d));
	}

	void ArchiveDate::setDate(ExifDate& d) {
		m_date = d;
	}


	bool ArchiveDate::getUseEXIFDate() {
		return m_useEXIFDate;
	}

	void ArchiveDate::setUseEXIFDate(bool b) {
		setFalse();
		m_useEXIFDate = true;
	}

	bool ArchiveDate::getUseFileDate() {
		return m_useFileDate;
	}

	void ArchiveDate::setUseFileDate(bool b) {
		setFalse();
		m_useFileDate = true;
		
	}

	bool ArchiveDate::getUseDate() {
		return m_useDate;
	}
	void ArchiveDate::setUseDate(ExifDate date) {
		setFalse();
		m_useDate = true;
	}

	bool ArchiveDate::getUseDateToday() {
		return m_useDateToday;
	}

	void ArchiveDate::setUseDateToday(bool b) {
		setFalse();
		m_useDateToday = true;
	}

	bool ArchiveDate::process(ImageContainer &ic) {
		const MetadataObject *picMetadata = ic.getPictureMetadata();
		const MetadataObject *rawMetadata = ic.getRawMetadata();
		
		BasicExif *picId = nullptr;
		BasicExif *rawId = nullptr;
		if (ic.hasPictureFile()) {
			picId = (BasicExif *)&(ic.getPictureId());
		}
		if (ic.hasRawFile()) {
			rawId = (BasicExif *)&(ic.getRawId());
		}
		if (rawMetadata != nullptr) {
			const ExifDateTime captureDate = rawMetadata->getCaptureDate();
			m_exifDate.reset(new ExifDate(captureDate));
			if (m_exifDate->isOk() == true) {
				return true;	// Found date
			}
		} else if (picMetadata != nullptr) {
			const ExifDateTime captureDate = picMetadata->getCaptureDate();
			m_exifDate.reset(new ExifDate(captureDate));
			if (m_exifDate->isOk() == true) {
				return true;
			}
		}
		if ((rawId != nullptr) && (rawId->isExifFound())) {
				const ExifDateTime& captureDate = rawId->getDateTimeDigitized();
				m_exifDate.reset(new ExifDate(captureDate));
				if (m_exifDate->isOk() == true) {
					return true;
				}
		} else if ((picId != nullptr) && (picId->isExifFound())) {
				const ExifDateTime& captureDate = picId->getDateTimeDigitized();
				m_exifDate.reset(new ExifDate(captureDate));
				if (m_exifDate->isOk() == true) {
					return true;
				}
		}
		return false;
	}

	ExifDate ArchiveDate::getArchiveDate() {
		if (m_useDate) {
			m_achiveDate = m_date;
			return m_achiveDate;
		}
		if (m_exifDate->isOk() == false) {
			if (m_fileDate != nullptr) {
				m_achiveDate = *m_fileDate;
			}
			else {
				m_achiveDate = m_date;
			}
		}
		if (m_useDateToday) {
			m_date.now();
			m_achiveDate = m_date;
		} else if (m_useEXIFDate && m_exifDate != nullptr) {
			m_achiveDate = *m_exifDate;
		} else if (m_useFileDate && m_fileDate != nullptr) {
			m_achiveDate = *m_fileDate;
		}
		return m_achiveDate;
	}

	void ArchiveDate::setFalse() {
		m_useEXIFDate = false;
		m_useFileDate = false;
		m_useDate = false;
		m_useDateToday = false;
	}

	void ArchiveDate::processExifDate() {

	}

	void ArchiveDate::processFileDate(const char *path) {

	}
	
}