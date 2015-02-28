#include "ArchiveDate.h"
#include "ImageContainer.h"
#include "MetadataObject.h"
#include "ExifDate.h"
#include "ImageId.h"

namespace simplearchive {

	bool ArchiveDate::m_useEXIFDate = true;
	bool ArchiveDate::m_useFileDate = false;
	bool ArchiveDate::m_useDate = false;
	bool ArchiveDate::m_useDateToday = false;
	ExifDate ArchiveDate::m_date;

	ArchiveDate::ArchiveDate()
	{
		m_exifDate = nullptr;
		m_fileDate = nullptr;
	}


	ArchiveDate::~ArchiveDate()
	{
		if (m_exifDate) {
			delete m_exifDate;
		}
		if (m_fileDate) {
			delete m_fileDate;
		}
		
	}

	void ArchiveDate::setExitDate(ExifDate& d) {
		if (m_exifDate) {
			*m_exifDate = d;
		} else {
			m_exifDate = new ExifDate(d);
		}
	}

	void ArchiveDate::setFileDate(ExifDate& d) {
		if (m_fileDate) {
			*m_fileDate = d;
		}
		else {
			m_fileDate = new ExifDate(d);
		}
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
		const MetadataObject *rawNetadata = ic.getRawMetadata();
		const CImageId *picId = ic.getPictureId();
		const CImageId *rawId = ic.getRawId();

		if (rawNetadata != nullptr) {
			std::string captureDate = rawNetadata->getCaptureDate();
			m_exifDate = new ExifDate(captureDate.c_str());
			if (m_exifDate->isOk() == false) {
				return false;
			}
		} else if (picMetadata != nullptr) {
			std::string captureDate = picMetadata->getCaptureDate();
			m_exifDate = new ExifDate(captureDate.c_str());
			if (m_exifDate->isOk() == false) {
				return false;
			}
		}
		if ((rawId != nullptr) && (rawId->isExifFound())) {
				const ExifDateTime& captureDate = rawId->getDateTimeDigitized();
				m_exifDate = new ExifDate(captureDate);
				if (m_exifDate->isOk() == false) {
					return false;
				}
		} else if ((picId != nullptr) && (picId->isExifFound())) {
				const ExifDateTime& captureDate = picId->getDateTimeDigitized();
				m_exifDate = new ExifDate(captureDate);
				if (m_exifDate->isOk() == false) {
					return false;
				}
		}
		return true;
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