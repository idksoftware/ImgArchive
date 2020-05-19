#include "ResultsPresentation.h"
#include "ResultsList.h"
#include <iostream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"

namespace simplearchive {


bool ResultsPresentation::write(FormatType formatType) {
	switch (formatType) {
	case FormatType::XML:
		return writeXML();
	case FormatType::Json:
		return writeJson();
	case FormatType::Human:
		return writeHuman();
	case FormatType::csv:
		return writeCSV();
	case FormatType::Html:
		return writeHtml();

	case FormatType::unknown:
	default:
		break;
	}
	return false;
}

bool ResultsPresentation::iequals(const std::string& a, const std::string& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

ResultsPresentation::FormatType ResultsPresentation::parse(const char* str) {
	std::string typeStr = str;

	if (iequals(typeStr, "XML")) {
		return FormatType::XML;
	}
	else if (iequals(typeStr, "Json")) {
		return FormatType::Json;
	}
	else if (iequals(typeStr, "csv")) {
		return FormatType::csv;
	}
	else if (iequals(typeStr, "human")) {
		return FormatType::Human;
	}
	else if (iequals(typeStr, "html")) {
		return FormatType::Html;
	}
	return FormatType::unknown;
}

std::string ResultsPresentation::writeTag(const char* tag, const std::string& value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	if (!value.empty() && (value.compare("null") != 0)) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	}
	else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}

std::string ResultsPresentation::writeTag(const char* tag, const int value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	return xml.str();
}

bool ResultsPresentation::writeHuman() {
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

	for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
		MTSchema& columnInfo = *i;
		printf("%s ", columnInfo.getName().c_str());

	}
	printf("\n");
	for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
		SharedMTRow row = *rowIt;
		for (auto i = row->begin(); i != row->end(); i++) {
			SharedMTColumn column = *i;
			//std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
			//std::cout << mtSchema->getName();
			//if (mtSchema == nullptr) {
			//	continue;
			//}
			std::cout << ',';
			std::cout << column->toString();

		}
		std::cout << '\n';
	}
	//}
	return true;
}


} /* namespace simplearchive */
