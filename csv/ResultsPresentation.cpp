#include "ResultsPresentation.h"
#include "ResultsList.h"
#include <iostream>
#include <fstream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"
#include <iomanip>
#include "DBDefines.h"
#include "HistoryEvent.h"

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
	size_t sz = a.size();
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

bool ResultsPresentation::write(std::string& output) {
	if (m_useFile) {
		std::ofstream outfile;
		outfile.open(m_filename.c_str());
		if (outfile.is_open() == false) {
			return false;
		}
		outfile << output;
		outfile.close();
	}
	else {
		std::cout << output;
	}
}
/*
bool WriteHuman::write()
{
	ColumnJustification columnJustification(m_resultsList.getTableSchema().size());

	for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
		SharedMTRow row = *rowIt;
		columnJustification.readRow(row);
	}
	for (int i = 0; i < m_resultsList.getTableSchema().size(); i++) {
		std::cout << ' ' << columnJustification.getSize(i) << '\n';
	}
	int idx = 0;
	for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {

		MTSchema& columnInfo = *i;
		std::string s = columnInfo.getName();
		columnJustification.header(idx, s);
		std::cout << std::setw(columnJustification.getSize(idx++) + 1) << columnInfo.getName();
	}
	printf("\n");
	for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
		SharedMTRow row = *rowIt;
		idx = 0;
		for (auto i = row->begin(); i != row->end(); i++) {
			SharedMTColumn column = *i;
			std::cout << std::setw(columnJustification.getSize(idx++) + 1) << column->toString();

		}
		std::cout << '\n';
	}
	return true;
}
*/


std::string WriteXML::writeTag(const char* tag, const std::string& value, int tab) {
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

std::string WriteXML::writeTag(const char* tag, const int value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	return xml.str();
}



std::string WriteJSON::writeArrayOpen(const char* tag) {
	std::ostringstream json;
	json << '\"' << tag << "\": [\n";
	return json.str();
}

std::string WriteJSON::writeArrayClose(bool end) {
	std::ostringstream json;
	if (end) {
		json << "]\n";
	}
	else {
		json << "],\n";
	}
	return json.str();
}

std::string WriteJSON::writeTag(const char* tag, const std::string& value, bool end) {
	std::ostringstream json;
	if (end) {
		json << '\"' << tag << "\": \"" << value << "\"\n";
	}
	else {
		json << '\"' << tag << "\": \"" << value << "\",\n";
	}
	return json.str();
}

std::string WriteBase::getOutput() {
	return m_output.str();
}

} /* namespace simplearchive */
