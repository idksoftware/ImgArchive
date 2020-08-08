


#include <iostream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"
#include <iomanip>
#include "DBDefines.h"
#include "HistoryEvent.h"
#include "SystemHistoryResultsPresentation.h"
#include "ResultsList.h"
#include "ResultsPresentation.h"

namespace simplearchive {

	//
	// WriteHuman
	//
	class  SystemHistoryWriteHuman : public WriteHuman {

	public:
		SystemHistoryWriteHuman(ResultsList& resultsList);
		~SystemHistoryWriteHuman() = default;

		bool write() override;
	};

	SystemHistoryWriteHuman::SystemHistoryWriteHuman(ResultsList& resultsList) : WriteHuman(resultsList) {}

	bool SystemHistoryWriteHuman::write() {
	
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
				m_output << std::setw(HistoryEvent::maxStringSize() + 1) << columnInfo.getName();
			}
			else {
				m_output << std::setw(columnJustification.getSize(idx) + 1) << columnInfo.getName();
			}
			idx++;
		}
		m_output << "\n";
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			idx = 0;
			for (auto i = row->begin(); i != row->end(); i++) {
				SharedMTColumn column = *i;
				if (eventIdx == idx) {
					HistoryEvent::Event evn = static_cast<HistoryEvent::Event>(column->getInt());
					m_output << std::setw(HistoryEvent::maxStringSize() + 1) << HistoryEvent::getString(evn);
				}
				else {
					m_output << std::setw(columnJustification.getSize(idx) + 1) << column->toString();
				}
				idx++;
			}
			m_output << '\n';
		}
		return true;
	}

	bool SystemHistoryResultsPresentation::writeHuman() {
		SystemHistoryWriteHuman systemHistoryWriteHuman(m_resultsList);
		if (!systemHistoryWriteHuman.write()) {
			return false;
		};
		if (write(systemHistoryWriteHuman.getOutput()) == false) {
			return false;
		}
		return true;
	}

	//
	// WriteJSON
	//
	class  SystemHistoryWriteJson : public WriteJSON {
	public:
		SystemHistoryWriteJson(ResultsList& resultsList);
		~SystemHistoryWriteJson() = default;
		bool write() override;
	};

	SystemHistoryWriteJson::SystemHistoryWriteJson(ResultsList& resultsList) : WriteJSON(resultsList) {}

	bool SystemHistoryWriteJson::write() {
		MTTableSchema& tableSchema = m_resultsList.getTableSchema();
		int rowSize = tableSchema.size();

		m_output << "{\n";
		m_output << "\"images\": [\n";
		bool first = true;
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			int idx = 0;
			if (first) {
				first = false;
			}
			else {
				m_output << "},\n";
			}
			m_output << "{\n";
			for (auto i = row->begin(); i != row->end(); i++) {
				SharedMTColumn column = *i;
				MTSchema& col = tableSchema.at(idx);
				std::string tag = col.getName();
				m_output << writeTag(tag.c_str(), column->toString(), (idx >= rowSize - 1));
				idx++;
			}

		}
		m_output << "}\n";
		m_output << "] }\n";
		return true;
	};

	bool SystemHistoryResultsPresentation::writeJson() {
		SystemHistoryWriteJson systemHistoryWriteJson(m_resultsList);
		if (!systemHistoryWriteJson.write()) {
			return false;
		};
		if (write(systemHistoryWriteJson.getOutput()) == false) {
			return false;
		}
		return true;
	}

	//
	// WriteHtml
	//
	class  SystemHistoryWriteHtml : public WriteHtml {
	public:
		SystemHistoryWriteHtml(ResultsList& resultsList);
		~SystemHistoryWriteHtml() = default;
		bool write() override;
	};

	SystemHistoryWriteHtml::SystemHistoryWriteHtml(ResultsList& resultsList) : WriteHtml(resultsList) {}

	bool SystemHistoryWriteHtml::write() {

		m_output << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "<title>Title of the document</title>\n"
			<< "<link rel = \"stylesheet\" href = \"../../../../config/styles.css\">\n"
			<< "<style> \n"
			<< "	table{ \n"
			<< "		font-family: arial, sans - serif;\n"
			<< "		border-collapse: collapse;\n"
			<< "		width: 100 %;\n"
			<< "	}\n"
			<< "	td, th{ \n"
			<< "	border: 1px solid #dddddd;\n"
			<< "		text-align: left;\n"
			<< "		padding: 8px;\n"
			<< "	}\n"
			<< "	tr:nth-child(even) {\n"
			<< "		background-color: #dddddd;\n"
			<< "	}\n"
			<< "</style> \n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<div class=\"file_block\">\n"
			<< "<table class=\"image_attr\">\n";
		m_output << "<tr>\n";
		int eventIdx = 0;
		int idx = 0;
		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
			MTSchema& columnInfo = *i;
			std::string name = columnInfo.getName();
			if (name.compare(DB_EVENT) == 0) {
				eventIdx = idx;
			}
			m_output << "<th>" << name << "</th>\n";
			idx++;
		}
		m_output << "</tr>\n";

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			m_output << "<tr>\n";
			int idx = 0;
			for (size_t i = 0; i != row->size(); i++) {

				if (eventIdx == i) {
					int evnInt = row->columnAt((int)i).getInt();
					HistoryEvent::Event evn = static_cast<HistoryEvent::Event>(evnInt);
					m_output << "<td>" << HistoryEvent::getString(evn) << "</td>\n";
				}
				else {
					m_output << "<td>" << row->columnAt((int)i).toString() << "</td>\n";
				}
				idx++;
			}
			m_output << "</tr>\n";
		}
		m_output << "</table>\n"
			<< "</div>\n"
			<< "</body>\n"
			<< "</html>\n";
		return true;
	};

	bool SystemHistoryResultsPresentation::writeHtml()
	{
		SystemHistoryWriteHtml systemHistoryWriteHtml(m_resultsList);
		if (!systemHistoryWriteHtml.write()) {
			return false;
		}
		if (write(systemHistoryWriteHtml.getOutput()) == false) {
			return false;
		}
		return true;
	}

	class  SystemHistorywriteCSV : public WriteCSV {

	public:
		SystemHistorywriteCSV(ResultsList& resultsList);
		~SystemHistorywriteCSV() = default;

		bool write() override;
	};

	SystemHistorywriteCSV::SystemHistorywriteCSV(ResultsList& resultsList) : WriteCSV(resultsList) {}

	bool SystemHistorywriteCSV::write() {
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			m_output << (*rowIt)->toString() << '\n';
		}
		return true;
	}

	bool SystemHistoryResultsPresentation::writeCSV() {
		SystemHistorywriteCSV systemHistorywriteCSV(m_resultsList);
		if (!systemHistorywriteCSV.write()) {
			return false;
		}
		if (write(systemHistorywriteCSV.getOutput()) == false) {
			return false;
		}
		return true;
	}

	//
	// WriteXML
	//
	class SystemHistoryWriteXML : public WriteXML {
	public:
		SystemHistoryWriteXML(ResultsList& resultsList);
		~SystemHistoryWriteXML() = default;

		bool write() override;
	};

	SystemHistoryWriteXML::SystemHistoryWriteXML(ResultsList& resultsList) : WriteXML(resultsList) {}

	bool SystemHistoryWriteXML::write() {
		m_output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";


		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			m_output << "\t<Event>\n";
			for (size_t i = 0; i != row->size(); i++) {
				MTTableSchema tableSchema = m_resultsList.getTableSchema();
				std::string name = tableSchema.getColumnName(i).c_str();
				if (name.compare(DB_EVENT) == 0) {
					int evnIdx = row->columnAt((int)i).getInt();
					HistoryEvent::Event evn = static_cast<HistoryEvent::Event>(evnIdx);
					m_output << writeTag(name.c_str(), HistoryEvent::getString(evn), 2);
				}
				else {
					m_output << writeTag(name.c_str(), row->columnAt((int)i).toString(), 2);
				}
			}
			m_output << "\t</Event>\n";
		}

		m_output << "</History>\n";
		return true;
	}

	bool SystemHistoryResultsPresentation::writeXML() {
		SystemHistoryWriteXML systemHistoryWriteXML(m_resultsList);
		if (!systemHistoryWriteXML.write()) {
			return false;
		}
		if (write(systemHistoryWriteXML.getOutput()) == false) {
			return false;
		}
		return true;
	}
}; // namespace