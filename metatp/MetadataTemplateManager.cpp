#include "MetadataTemplateManager.h"
#include "CLogger.h"

bool MetadataTemplateManager::metadataTemplate(bool repo, bool templateFile) {
	/*
	CLogger& logger = CLogger::getLogger();
	AppConfig& config = AppConfig::get();
	std::string templatePath = config.getTemplatePath();

	if (templatePath.empty()) {
		logger.log(LOG_OK, CLogger::Level::INFO, "Cannot find template path");
		return false;
	}
	std::string metatemplatePath = templatePath;
	metatemplatePath += "/master.tpl";

	if (MetadataTemplate::read(templatePath.c_str(), "master.tpl") == false) {
		logger.log(LOG_OK, CLogger::Level::INFO, "Cannot read master template file \"%s\"", metatemplatePath.c_str());
	}
	logger.log(LOG_OK, CLogger::Level::INFO, "Completed reading metadata template file \"%s\"", metatemplatePath.c_str());
	*/
	return true;
}
