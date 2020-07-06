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
#include "CLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	bool ArchiveDate::m_useEXIFDate = true;
	bool ArchiveDate::m_useFileDate = false;
	bool ArchiveDate::m_useThisDate = false;
	bool ArchiveDate::m_useDateToday = false;
	bool ArchiveDate::m_forceDate = false;
	bool ArchiveDate::m_defaultDateSet = false;
	ExifDate ArchiveDate::m_date;
	

	ArchiveDate::ArchiveDate() : 
					m_exifDate(nullptr), m_fileDate(nullptr)
	{}


	ArchiveDate::~ArchiveDate()
	{
	}

	void ArchiveDate::setExifDate(const ExifDate& d) {
		m_exifDate = std::make_shared<ExifDate>(d);
	}

	void ArchiveDate::setFileDate(const ExifDate& d) {
		m_fileDate = std::make_shared<ExifDate>(d);
	}

	void ArchiveDate::setDate(const ExifDate& d) {
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
		return m_useThisDate;
	}
	void ArchiveDate::setUseDate(const ExifDate &date) {
		setFalse();
		m_useThisDate = true;
		m_date = date;
	}

	bool ArchiveDate::getUseDateToday() {
		return m_useDateToday;
	}

	void ArchiveDate::setUseDateToday(bool b) {
		setFalse();
		m_useDateToday = true;
	}

	bool ArchiveDate::process(ImageContainer &ic) {
		CLogger &logger = CLogger::getLogger();

		const MetadataObject *picMetadata = ic.getPictureMetadata();
		const MetadataObject *rawMetadata = ic.getRawMetadata();

		BasicMetadata *picId = nullptr;
		BasicMetadata *rawId = nullptr;
		if (ic.hasPictureFile()) {
			//logger.log(LOG_OK, CLogger::Level::FINE, "Image \"%s\" has picture file: ", ic.getName());
			picId = (BasicMetadata *)&(ic.getPictureId());
			const ExifDateTime& createTime = picId->getCreateTime();
			m_fileDate = std::make_shared<ExifDate>(createTime);
			//logger.log(LOG_OK, CLogger::Level::TRACE, "Image \"%s\" will be archived under the date: %s", ic.getName(), createTime.toString().c_str());
			m_usingDate = USING_FILE_PIC_DATE;
		}
		else if (ic.hasRawFile()) {
			//logger.log(LOG_OK, CLogger::Level::FINE, "Image \"%s\" has RAW file: ", ic.getName());
			rawId = (BasicMetadata *)&(ic.getRawId());
			const ExifDateTime& createTime = rawId->getCreateTime();
			m_fileDate = std::make_shared<ExifDate>(createTime);
			//logger.log(LOG_OK, CLogger::Level::TRACE, "Image \"%s\" will be archived under the date: %s", ic.getName(), createTime.toString().c_str());
			m_usingDate = USING_FILE_RAW_DATE;
		}
	
		if (rawMetadata != nullptr) {
			try {
				if (rawMetadata->isCaptureDateNull() == false) {
					const ExifDateTime captureDate = rawMetadata->getCaptureDate();
					m_exifDate.reset(new ExifDate(captureDate));
					if (m_exifDate->isOk() == true) {
						//logger.log(LOG_OK, CLogger::Level::SUMMARY, "Image \"%s\" will be archived under the date: %s", ic.getName(), m_exifDate->toString().c_str());
						m_usingDate = USING_CAPTURE_RAW_DATE;
						return true;	// Found date
					}
					m_usingDate = UNKNOWN;
				}
				m_usingDate = UNKNOWN;
			}
			catch (MTTypeException e) {
				//logger.log(LOG_OK, CLogger::Level::TRACE, "No picture digitized date?", ic.getName());
				m_usingDate = UNKNOWN;
			}
			
		} 
		if ((rawId != nullptr) && (rawId->isExifFound())) {
			try {
				const ExifDateTime& captureDate = rawId->getDateTimeDigitized();
				m_exifDate = std::make_shared<ExifDate>(captureDate);
				if (m_exifDate->isOk() == true) {
					//logger.log(LOG_OK, CLogger::Level::SUMMARY, "Image \"%s\" will be archived under the date: %s", ic.getName(), m_exifDate->toString().c_str());
					m_usingDate = USING_CAPTURE_RAW_DATE;
					return true;
				}
				m_usingDate = UNKNOWN;
			}
			catch (MTTypeException e) {
				//logger.log(LOG_OK, CLogger::Level::TRACE, "No raw digitized date?", ic.getName());
				m_usingDate = UNKNOWN;
			}
		}
		if (picMetadata != nullptr) {
			try {
				if (picMetadata->isCaptureDateNull() == false) {
					const ExifDateTime captureDate = picMetadata->getCaptureDate();
					m_exifDate = std::make_shared<ExifDate>(captureDate);
					if (m_exifDate->isOk() == true) {
						//logger.log(LOG_OK, CLogger::Level::SUMMARY, "Image \"%s\" will be archived under the date: %s", ic.getName(), m_exifDate->toString().c_str());
						m_usingDate = USING_CAPTURE_PIC_DATE;
						return true;
					}
				}
				m_usingDate = UNKNOWN;
			}
			catch (MTTypeException e) {
				//logger.log(LOG_OK, CLogger::Level::TRACE, "No picture capture date?", ic.getName());
				m_usingDate = UNKNOWN;
			}
		}
			
		
		if ((picId != nullptr) && (picId->isExifFound())) {
			try {
				if (picId->getDateTimeDigitizedNull() == false) {
					const ExifDateTime& captureDate = picId->getDateTimeDigitized();
					m_exifDate = std::make_shared<ExifDate>(captureDate);
					if (m_exifDate->isOk() == true) {
						//logger.log(LOG_OK, CLogger::Level::SUMMARY, "Image \"%s\" will be archived under the date: %s", ic.getName(), m_exifDate->toString().c_str());
						m_usingDate = USING_CAPTURE_PIC_DATE;
						return true;
					}
				}
				m_usingDate = UNKNOWN;
			}
			catch (MTTypeException e) {
				//logger.log(LOG_OK, CLogger::Level::TRACE, "Image \"%s\" No picture digitized date?", ic.getName());
				m_usingDate = UNKNOWN;
			}
		}
		return false;
	}

	ExifDate ArchiveDate::getArchiveDate() {
		if (m_forceDate == true) {
			if (m_useThisDate) {
				m_achiveDate = m_date;
				m_usingDate = USING_FORCED_DATE;
				return m_achiveDate;
			}
			if (m_useDateToday) {
				m_date.now();
				m_achiveDate = m_date;
				m_usingDate = USING_FORCED_DATE_TODAY;
			}
			if (m_useFileDate) {
				m_achiveDate = *m_fileDate;
				m_usingDate = USING_FORCED_FILE_DATE;
			}
		}
		else {
			
			if (m_exifDate == nullptr || m_exifDate->isOk() == false) {
				if (m_fileDate != nullptr) {
					m_achiveDate = *m_fileDate;
					m_usingDate = USING_FILE_DATE;
				}
				else {
					if (getDefaultDateSet() == true) {
						m_achiveDate = m_date;
						m_usingDate = USING_DEFAULT_DATE;
					}
					else {
						m_date.now();
						m_achiveDate = m_date;
						m_usingDate = USING_DATE_TODAY;
					}
				}
			}
			if (m_useDateToday) {
				m_date.now();
				m_achiveDate = m_date;
				m_usingDate = USING_DATE_TODAY;
			}
			else if (m_useEXIFDate && m_exifDate != nullptr) {
				m_achiveDate = *m_exifDate;
				m_usingDate = USING_EXIF_DATE;
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
		m_useThisDate = false;
		m_useDateToday = false;
	}

	void ArchiveDate::processExifDate() {

	}

	void ArchiveDate::processFileDate(const char *path) {

	}
	
	std::string ArchiveDate::getUsingDateString() {
		switch (m_usingDate) {
		case ArchiveDate::USING_FORCED_DATE:
			return "Using forced date";
		case ArchiveDate::USING_FORCED_DATE_TODAY:
			return "Using forced date today";
		case ArchiveDate::USING_FORCED_FILE_DATE:
			return "Using forced file date";
		case ArchiveDate::USING_DATE_TODAY:
			return "Using date today";
		case ArchiveDate::USING_DEFAULT_DATE:
			return "Using default date";
		case ArchiveDate::USING_FILE_DATE:
			return "Using file date";
		case ArchiveDate::USING_EXIF_DATE:
			return "Using exif date";
		case ArchiveDate::USING_CAPTURE_RAW_DATE:
			return "Using capture raw date";
		case ArchiveDate::USING_CAPTURE_PIC_DATE:
			return "Using capture pic date";
		case ArchiveDate::USING_FILE_RAW_DATE:
			return "Using file raw date";
		case ArchiveDate::USING_FILE_PIC_DATE:
			return "Using file pic date";
		case ArchiveDate::UNKNOWN:
			return "UNKNOWN";
		}
		return "UNKNOWN";
	}
}
