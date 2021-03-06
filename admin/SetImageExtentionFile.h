#pragma once

#include <string>
namespace simplearchive {

	class ExtentionItem;

	class SetImageExtentionFile
	{
	public:
		enum class Error {
			NoError,
			OperationFailed,
			WriteFailed,
			Unknown
		};
	private:
		Error m_error;
	public:
		SetImageExtentionFile() = default;
		virtual ~SetImageExtentionFile() = default;
		bool update(const char *ext, const char* type, const char* mimeType, const char* description);
		bool add(ExtentionItem& extentionItem);
		bool update(ExtentionItem& extentionItem);
		bool remove(const char *ext);
		static bool validateOptions(const char* opts);
		Error getError() {
			return m_error;
		}
	};
};

