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

#ifndef IMAGEEXTENTIONS_H_
#define IMAGEEXTENTIONS_H_

#include <string>
#include <memory>
#include "ImageType.h"
#include "CSVArgs.h"

namespace simplearchive
{
	class CExtentionsFile;

	enum class AllowSelectionType
	{
		Raw,
		Picture,
		All
	};


	class ExtentionItem
	{
		std::string m_ext;
		ImageType m_type;
		std::string m_mimeType;
		std::string m_desciption;
	public:
		ExtentionItem() noexcept
		{
			m_ext = "";
			m_type = ImageType::Type::UNKNOWN_EXT;
		}

		ExtentionItem(const char* row, char delim = ':')
		{
			std::string data = row;
			//printf("%s", data.c_str());

			CSVArgs csvArgs(delim);
			csvArgs.process(row);
			m_ext = csvArgs.at(0);
			std::string typeStr = csvArgs.at(1);
			m_type = typeStr.c_str();
			m_mimeType = csvArgs.at(2);
			m_desciption = csvArgs.at(3);
		}

		ExtentionItem(std::string& ext,
		              ImageType type,
		              std::string& mimeType,
		              std::string& desciption)
		{
			m_ext = ext;
			m_type = type;
			m_mimeType = mimeType;
			m_desciption = desciption;
		}

		const std::string& getDesciption() const
		{
			return m_desciption;
		}

		bool isValid()
		{
			if (m_type.getType() == ImageType::Type::UNKNOWN_EXT)
			{
				return false;
			}
			if (m_ext.empty())
			{
				return false;
			}
			if (m_mimeType.empty())
			{
				return false;
			}
			return true;
		}

		void setDesciption(const std::string& desciption)
		{
			m_desciption = desciption;
		}

		const std::string& getExt()
		{
			return m_ext;
		}

		void setExt(const std::string& ext)
		{
			m_ext = ext;
		}

		const std::string& getMimeType() const
		{
			return m_mimeType;
		}

		void setMimeType(const std::string& mimeType)
		{
			m_mimeType = mimeType;
		}

		ImageType getType()
		{
			return m_type;
		}

		void setType(ImageType& type)
		{
			m_type = type;
		}

		std::string toString();
	};

	typedef std::unique_ptr<ExtentionItem> ExtentionItem_Ptr;

	class ImageExtentions
	{
	private:
		ImageExtentions() = default;
		~ImageExtentions() = default;

		static bool m_once;
		static std::unique_ptr<CExtentionsFile> m_extentionsFile;
		static std::string m_extentionsFilePath;
		static bool m_isError;
	public:

		ImageType getType(const char* filename);
		ImageType findType(const char* ext);
		std::shared_ptr<ExtentionItem> find(const char* ext);
		bool insert(ExtentionItem& extentionItem);
		bool update(ExtentionItem& extentionItem);
		bool remove(const char* ext);
		static ImageExtentions& get();
		bool isAllowed(const char* ext);
		//bool IsValid(const char *filename);
		bool IsValidXML(const char* filename);
		bool write();

		static const std::string& getExtentionsFilePath()
		{
			return m_extentionsFilePath;
		}

		bool getList(std::vector<std::shared_ptr<ExtentionItem>>& list, AllowSelectionType selectionType);

		static bool setExtentionsFilePath(const char* extentionsFilePath);

		static bool isError()
		{
			return m_isError;
		}
	};
} /* namespace simplearchive */
#endif /* IMAGEEXTENTIONS_H_ */
