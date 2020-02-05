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
		bool update(ExtentionItem& extentionItem);

		static bool validateOptions(const char* opts);
	};
};

