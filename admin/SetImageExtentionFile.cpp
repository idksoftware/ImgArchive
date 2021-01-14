#include "SetImageExtentionFile.h"
#include "ImageExtentions.h"
#include "SAUtils.h"
#include "CSVArgs.h"

namespace simplearchive {

	bool SetImageExtentionFile::update(const char* ext, const char* type, const char* mimeType, const char* description)
	{
		ImageType _imageType;
		_imageType.fromString(type);
		std::string _ext(ext);
		std::string _mimeType(mimeType);
		std::string _description(description);

		ExtentionItem _extentionItem(_ext, _imageType, _mimeType, _description);
		bool ret = update(_extentionItem);
		return ret;
	}

	bool SetImageExtentionFile::add(ExtentionItem& extentionItem)
	{
		ImageExtentions& imageExtentions = ImageExtentions::get();
		bool ret = imageExtentions.insert(extentionItem);
		if (ret) {
			ret = imageExtentions.write();
			if (!ret) {
				m_error = Error::WriteFailed;
				return false;
			}
		}
		m_error = Error::OperationFailed;
		return ret;
	}

	bool SetImageExtentionFile::update(ExtentionItem &extentionItem)
	{
		ImageExtentions& imageExtentions = ImageExtentions::get();
		bool ret = imageExtentions.update(extentionItem);
		if (ret) {
			ret = imageExtentions.write();
			if (!ret) {
				m_error = Error::WriteFailed;
				return false;
			}
		}
		m_error = Error::OperationFailed;
		return ret;
	}

	bool SetImageExtentionFile::remove(const char *ext)
	{
		ImageExtentions& imageExtentions = ImageExtentions::get();
		bool ret = imageExtentions.remove(ext);
		if (ret) {
			ret = imageExtentions.write();
			if (!ret) {
				m_error = Error::WriteFailed;
				return false;
			}
		}
		m_error = Error::OperationFailed;
		return ret;
	}

	bool SetImageExtentionFile::validateOptions(const char *opts)
	{
		ExtentionItem extentionItem(opts, ',');
		if (!extentionItem.isValid()) {
			return false;
		}
		return true;
	}

};