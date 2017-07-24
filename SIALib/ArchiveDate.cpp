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
#include "BasicMetadataFactory.h"

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
	bool ArchiveDate::m_forceDate = false;
	bool ArchiveDate::m_defaultDateSet = false;
	ExifDate ArchiveDate::m_date;

	ArchiveDate::ArchiveDate()
	{
		m_exifDate.reset(0);
		m_fileDate.reset(0);
	}


	ArchiveDate::~ArchiveDate()
	{
	}

	void ArchiveDate::setExifDate(ExifDate& d) {
		m_exifDate.reset(new ExifDate(d));
	}

	void ArchiveDate::setFileDate(ExifDate& d) {
		m_fileDate.reset(new ExifDate(d));
	}

	void ArchiveDate::setDate(ExifDate& d) {
		m_date = d;
	}

	bool ArchiveDate::getForceDate() {
		return m_forceDate;
	}

	void ArchiveDate::setForceDate(bool b) {
		m_forceDate = b;
	}

	bool ArchiveDate::getUseEXIFDate() {
		return m_useEXIFDate;
	}

	void ArchiveDate::setUseEXIFDate(bool b) {
		setFalse();
		m_useEXIFDate = b;
	}

	bool ArchiveDate::getUseFileDate() {
		return m_useFileDate;
	}

	void ArchiveDate::setUseFileDate(bool b) {
		setFalse();
		m_useFileDate = b;
		
	}

	bool ArchiveDate::getDefaultDateSet() {
		return m_defaultDateSet;
	}

	void ArchiveDate::setDefaultDateSet(bool b) {
		setFalse();
		m_defaultDateSet = b;
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

		BasicMetadata *picId = nullptr;
		BasicMetadata *rawId = nullptr;
		if (ic.hasPictureFile()) {
			picId = (BasicMetadata *)&(ic.getPictureId());
			const ExifDateTime& createTime = picId->getCreateTime();
			m_fileDate.reset(new ExifDate(createTime));
		}
		else if (ic.hasRawFile()) {
			rawId = (BasicMetadata *)&(ic.getRawId());
			const ExifDateTime& createTime = rawId->getCreateTime();
			m_fileDate.reset(new ExifDate(createTime));
		}
		/* Add today as default 
		else {
			 
			//const ExifDateTime& createTime = *(new ExifDateTime);
		}
		*/
		if (rawMetadata != nullptr) {
			try {
				
				const ExifDateTime captureDate = rawMetadata->getCaptureDate();
				m_exifDate.reset(new ExifDate(captureDate));
				if (m_exifDate->isOk() == true) {
					return true;	// Found date
				}
			}
			catch (MTTypeException e) {
				printf("No raw capture date\n");
			}
			
		} 
		if ((rawId != nullptr) && (rawId->isExifFound())) {
			try {
				const ExifDateTime& captureDate = rawId->getDateTimeDigitized();
				m_exifDate.reset(new ExifDate(captureDate));
				if (m_exifDate->isOk() == true) {
					return true;
				}
			}
			catch (MTTypeException e) {
				printf("No raw digitized date\n");
			}
		}
		if (picMetadata != nullptr) {
			try {
				const ExifDateTime captureDate = picMetadata->getCaptureDate();
				m_exifDate.reset(new ExifDate(captureDate));
				if (m_exifDate->isOk() == true) {
					return true;
				}
			}
			catch (MTTypeException e) {
				printf("No picture capture date\n");
			}
		}
			
		
		if ((picId != nullptr) && (picId->isExifFound())) {
			try {
				const ExifDateTime& captureDate = picId->getDateTimeDigitized();
				m_exifDate.reset(new ExifDate(captureDate));
				if (m_exifDate->isOk() == true) {
					return true;
				}
			}
			catch (MTTypeException e) {
				printf("No picture digitized date\n");
			}
		}
		return false;
	}

	ExifDate ArchiveDate::getArchiveDate() {
		if (m_forceDate == true) {
			if (m_useDate) {
				m_achiveDate = m_date;
				return m_achiveDate;
			}
			if (m_useDateToday) {
				m_date.now();
				m_achiveDate = m_date;
			}
			if (m_useFileDate) {
				m_achiveDate = *m_fileDate;
			}
		}
		else {
			
			if (m_exifDate == nullptr || m_exifDate->isOk() == false) {
				if (m_fileDate != nullptr) {
					m_achiveDate = *m_fileDate;
				}
				else {
					if (getDefaultDateSet() == true) {
						m_achiveDate = m_date;
					}
					else {
						m_date.now();
						m_achiveDate = m_date;
					}
				}
			}
			if (m_useDateToday) {
				m_date.now();
				m_achiveDate = m_date;
			}
			else if (m_useEXIFDate && m_exifDate != nullptr) {
				m_achiveDate = *m_exifDate;
			}
			else if (m_useFileDate && m_fileDate != nullptr) {
				m_achiveDate = *m_fileDate;
			}
			
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