#pragma once

#include <string>
namespace simplearchive {

	class ExtentionItem;

	class SetImageExtentionFile
	{
	public:
		SetImageExtentionFile() = default;
		virtual ~SetImageExtentionFile() = default;
		bool update(const char *ext, const char* type, const char* mimeType, const char* description);
		bool add(ExtentionItem& extentionItem);
		bool update(ExtentionItem& extentionItem);
		bool remove(const char *ext);
		static bool validateOptions(const char* opts);
	};
};

