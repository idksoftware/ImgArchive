#include "ResultsPresentation.h"
#include "ResultsList.h"
#include <iostream>
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

	/*
	for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
		MTSchema& columnInfo = *i;
		printf("%s ", columnInfo.getName().c_str());

	}
	*/

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

CheckoutWriteHuman::CheckoutWriteHuman(ResultsList& resultsList) : WriteHuman(resultsList) {}

bool CheckoutWriteHuman::write()
{
	ColumnJustification columnJustification(m_resultsList.getTableSchema().size());

	for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
		SharedMTRow row = *rowIt;
		columnJustification.readRow(row);
	}
	int eventIdx = -1;
	int idx = 0;
	for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
		
		MTSchema& columnInfo = *i;
		std::string s = columnInfo.getName();
		columnJustification.header(idx, s);
		
		if (columnInfo.getName().compare(DB_EVENT) == 0) {
			eventIdx = idx;
			std::cout << std::setw(HistoryEvent::maxStringSize() + 1) << columnInfo.getName();
		}
		else {
			std::cout << std::setw(columnJustification.getSize(idx) + 1) << columnInfo.getName();
		}
		idx++;
	}
	printf("\n");
	for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
		SharedMTRow row = *rowIt;
		idx = 0;
		for (auto i = row->begin(); i != row->end(); i++) {
			SharedMTColumn column = *i;
			if (eventIdx == idx) {
				HistoryEvent::Event evn = static_cast<HistoryEvent::Event>(column->getInt());
				std::cout << std::setw(HistoryEvent::maxStringSize() + 1) << HistoryEvent::getString(evn);
			}
			else {
				std::cout << std::setw(columnJustification.getSize(idx) + 1) << column->toString();
			}
			idx++;
		}
		std::cout << '\n';
	}
	return true;
}



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


CheckoutWriteXML::CheckoutWriteXML(ResultsList& resultsList) : WriteXML(resultsList) {}

bool CheckoutWriteXML::write()
{
	std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<Catalog ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

	for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
		SharedMTRow row = *rowIt;
		std::cout << "\t<Event>\n";
		for (size_t i = 0; i != row->size(); i++) {
			MTTableSchema tableSchema = m_resultsList.getTableSchema();
			std::cout << writeTag(tableSchema.getColumnName(i).c_str(), row->columnAt((int)i).toString(), 2);
		}
		std::cout << "\t</Event>\n";
	}
	std::cout << "</Catalog>\n";
	return true;
	
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



CheckoutWriteJSON::CheckoutWriteJSON(ResultsList& resultsList) : WriteJSON(resultsList) {}


bool CheckoutWriteJSON::write()
{
	MTTableSchema& tableSchema = m_resultsList.getTableSchema();
	int rowSize = tableSchema.size();
	std::string output;
	output = "{\n";
	output += "\"images\": [\n";
	bool first = true;
	for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
		SharedMTRow row = *rowIt;
		int idx = 0;
		if (first) {
			first = false;
		}
		else {
			output += "},\n";
		}
		output += "{\n";
		for (auto i = row->begin(); i != row->end(); i++) {
			SharedMTColumn column = *i;
			MTSchema& col = tableSchema.at(idx);
			std::string tag = col.getName();
			output += writeTag(tag.c_str(), column->toString(), (idx >= rowSize - 1));
			idx++;
		}
		
	}
	output += "}\n";
	output += "] }\n";
	std::cout << output << "\n\r";
	return true;
}

} /* namespace simplearchive */
