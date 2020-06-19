#pragma once
//class MetadataTemplateManager
//{
//};

#include <map>
#include <mutex>
#include "ConfigReader.h"
#include "ResultsPresentation.h"
#include "MasterMetadataTemplate.h"
#include "DerivativeMetadataTemplate.h"

namespace simplearchive {

	class MetadataTemplateManager {


		static Config_ptr m_templateFile;
		static std::string& getValue(const char* key);
		static MasterMetadataTemplateRow m_masterMetadataTemplateRow;
		static DerivativeMetadataTemplateRow m_derivativeMetadataTemplateRow;

		MetadataTemplateManager() noexcept = default;
	public:
		static MetadataTemplateManager& GetInstance();
		virtual ~MetadataTemplateManager();

		MetadataTemplateManager(const MetadataTemplateManager& src) = delete;
		MetadataTemplateManager& operator=(const MetadataTemplateManager& rhs) = delete;

		static bool readMaster(const char* path, bool current);
		static bool readDerivative(const char* path, bool current);

		static bool readMaster(const char* path, const char* datafile);
		static bool readDerivative(const char* path, const char* datafile);
		static bool write(const char* datafile);
		static MetadataObject_ptr getMetadataObject();
	};

}