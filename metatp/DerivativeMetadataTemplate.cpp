
#include <stdio.h>
#include <iomanip>
#include "DBDefines.h"
#include "MetadataObject.h"
#include "MasterMetadataTemplate.h"
#include "DerivativeMetadataTemplate.h"

//#include "ConfigReader.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	DerivativeMetadataTemplateSchema DerivativeMetadataTemplateRow::m_tableSchema;

	std::unique_ptr<ConfigBlock>	DerivativeMetadataTemplate::m_templateFile(new ConfigBlock);
	DerivativeMetadataTemplateRow DerivativeMetadataTemplate::m_metadataTemplateRow;

	//Config *MetadataTemplate::m_templateFile = 0;


	DerivativeMetadataTemplate& DerivativeMetadataTemplate::GetInstance()
	{
		static DerivativeMetadataTemplate metadataTemplate;
		return metadataTemplate;
	}

	DerivativeMetadataTemplate::~DerivativeMetadataTemplate() {}

	static std::string defaultStr = "";


	std::string& DerivativeMetadataTemplate::getValue(const char* key) {
		std::map<std::string, std::string>::iterator it;

		if ((it = m_templateFile->find(key)) == m_templateFile->end()) {
			return defaultStr;
		}
		return it->second;
	}

	bool DerivativeMetadataTemplate::read(const char* datafile) {

		ConfigReader configReader;
		if (configReader.read(datafile, *m_templateFile) == false) {
			return false;
		}
		//templateFile.printAll();
		for (std::map<std::string, std::string>::iterator ii = m_templateFile->begin(); ii != m_templateFile->end(); ++ii) {
			std::string& value = getValue((*ii).first.c_str());
			//printf("\"%s\" = \"%s\"\n", (*ii).first.c_str(), (*ii).second.c_str());
			if (value.compare("")) {
				value = (*ii).second.c_str();
			}
			try {
				MTColumn& col = m_metadataTemplateRow.columnAt((*ii).first.c_str());
				col = (*ii).second.c_str();
			}
			catch (std::invalid_argument& e) {
				printf("%s", e.what());
			}
			catch (MTTypeException& e) {
				const char* tmp = e.what();
				printf("%s\n", tmp);
			}
		}

		MasterMetadataTemplateResultsPresentation resultsPresentation(m_metadataTemplateRow);
		resultsPresentation.writeHuman();

		return true;
	}

	bool DerivativeMetadataTemplate::write(const char* datafile) {
		return true;
	}

	/*
	DerivativeMetadataObject_ptr DerivativeMetadataTemplate::getMetadataObject() {
		DerivativeMetadataObject_ptr metadataObject = std::make_unique<DerivativeMetadataObject>();

		for (std::map<std::string, std::string>::iterator ii = m_templateFile->begin(); ii != m_templateFile->end(); ++ii) {
			//std::string &value = getValue((*ii).first.c_str());
			//printf("\"%s\" opt:\"%s\"\n", (*ii).first.c_str(), (*ii).second.c_str());
			try {
				MTColumn& col = metadataObject->columnAt((*ii).first.c_str());
				col = (*ii).second.c_str();
			}
			catch (std::invalid_argument& e) {
				printf("%s", e.what());
			}
			catch (MTTypeException& e) {
				const char* tmp = e.what();
				printf("%s\n", tmp);
			}
			//if (value.compare("")) {
			//	*value = (*ii).second.c_str();
			//}
		}
		
		return metadataObject;
	}
	*/

	bool DerivativeMetadataTemplateResultsPresentation::writeHuman()
	{
		/*
		std::ofstream file;
		if (!m_filename.empty()) {
			file.open(m_filename.c_str());
			if (file.is_open() == false) {
				return false;
			}
			file << "\n---------------------------------------------------\n";
			file << "Image: " << m_title << '\n';
			file << "Path : " << m_description << '\n';
			file << "=====================================================\n";
			file << "Date Time             version     Event      Comment\n\n";
			for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
				//std::cout << *i << '\n';
				CSVArgs csvArgs(',');
				if (csvArgs.process(i->c_str()) == false) {
					return false;
				}

				file << csvArgs.at(0) << "    ";
				file << csvArgs.at(1) << "      ";
				file << csvArgs.at(4) << "  ";
				file << csvArgs.at(3) << "\n\n";
			}
		}
		else {
		*/
		//std::cout << "\n---------------------------------------------------\n";
		//std::cout << "Image: " << m_title << '\n';
		//std::cout << "Path : " << m_description << '\n';
		//std::cout << "=====================================================\n";
		//std::cout << "Date Time             version     Event      Comment\n\n";

		std::cout << "\n";
		std::cout << "Derivative Metadata Template\n";
		std::cout << "============================\n";
		std::cout << "\n";
		for (auto i = m_row.begin(); i != m_row.end(); i++) {
			SharedMTColumn column = *i;
			std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
			std::cout << std::setw(20) << mtSchema->getName();
			std::cout << ": ";
			std::cout << column->toString();
			std::cout << "\n";
		}
		std::cout << '\n';

		//}
		return true;
	}
	bool DerivativeMetadataTemplateResultsPresentation::writeXML()
	{
		return false;
	}
	bool DerivativeMetadataTemplateResultsPresentation::writeCSV()
	{
		return false;
	}
	bool DerivativeMetadataTemplateResultsPresentation::writeJson()
	{
		return false;
	}
	bool DerivativeMetadataTemplateResultsPresentation::writeHtml()
	{
		return false;
	}
} /* namespace simplearchive */
