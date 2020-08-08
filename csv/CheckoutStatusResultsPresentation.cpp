

#include <iostream>
#include <fstream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"
#include <iomanip>
#include "DBDefines.h"
#include "HistoryEvent.h"
#include "CheckoutStatusResultsPresentation.h"
#include "ResultsList.h"
#include "ResultsPresentation.h"

namespace simplearchive {



	class CheckoutWriteHuman : public WriteHuman {

	public:
		CheckoutWriteHuman(ResultsList& resultsList);
		~CheckoutWriteHuman() = default;

		bool write() override;
	};

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

	bool CheckoutStatusResultsPresentation::writeHuman() {

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


		CheckoutWriteHuman writeHuman(m_resultsList);
		if (!writeHuman.write()) {
			return false;
		};
		if (write(writeHuman.getOutput()) == false) {
			return false;
		}
		return true;
	}

	class CheckoutWriteJSON : public WriteJSON {
	public:
		CheckoutWriteJSON(ResultsList& resultsList);
		~CheckoutWriteJSON() = default;

		bool write() override;
	};//

	CheckoutWriteJSON::CheckoutWriteJSON(ResultsList& resultsList) : WriteJSON(resultsList) {}


	bool CheckoutWriteJSON::write()
	{
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
	}

	bool CheckoutStatusResultsPresentation::writeJson() {
		CheckoutWriteJSON checkoutWriteJSON(m_resultsList);
		if (!checkoutWriteJSON.write()) {
			return false;
		};
		if (write(checkoutWriteJSON.getOutput()) == false) {
			return false;
		}
		return true;
	}

	
	
	class CheckoutWriteCSV : public WriteCSV {
	protected:
		
	public:
		CheckoutWriteCSV(ResultsList& resultsList) : WriteCSV{ resultsList } {}
		~CheckoutWriteCSV() = default;

		virtual bool write();
	};

	bool CheckoutWriteCSV::write() {
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			m_output << (*rowIt)->toString() << '\n';
		}
		return true;
	}
	
	bool CheckoutStatusResultsPresentation::writeCSV() {
		CheckoutWriteCSV checkoutWriteCSV(m_resultsList);
		if (!checkoutWriteCSV.write()) {
			return false;
		}
		if (write(checkoutWriteCSV.getOutput()) == false) {
			return false;
		}
		return true;
	}
	
	class CheckoutWriteXML : public WriteXML {
	public:
		CheckoutWriteXML(ResultsList& resultsList);
		~CheckoutWriteXML() = default;

		bool write() override;
	};


	CheckoutWriteXML::CheckoutWriteXML(ResultsList& resultsList) : WriteXML(resultsList) {}

	bool CheckoutWriteXML::write()
	{
		m_output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<Catalog ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			m_output << "\t<Event>\n";
			for (size_t i = 0; i != row->size(); i++) {
				MTTableSchema tableSchema = m_resultsList.getTableSchema();
				m_output << writeTag(tableSchema.getColumnName(i).c_str(), row->columnAt((int)i).toString(), 2);
			}
			m_output << "\t</Event>\n";
		}
		m_output << "</Catalog>\n";
		return true;
	}

	bool CheckoutStatusResultsPresentation::writeXML() {

		CheckoutWriteXML checkoutWriteXML(m_resultsList);
		if (!checkoutWriteXML.write()) {
			return false;
		}
		if (write(checkoutWriteXML.getOutput()) == false) {
			return false;
		}
		return true;
	}

	class CheckoutWriteHtml : public WriteHtml {
	protected:
		
	public:
		CheckoutWriteHtml(ResultsList& resultsList) : WriteHtml{ resultsList } {}
		~CheckoutWriteHtml() = default;

		virtual bool write();
	};


	bool CheckoutWriteHtml::write() {
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
	}

	bool CheckoutStatusResultsPresentation::writeHtml()
	{
		CheckoutWriteHtml checkoutWriteHtml(m_resultsList);
		if (!checkoutWriteHtml.write()) {
			return false;
		}
		if (write(checkoutWriteHtml.getOutput()) == false) {
			return false;
		}
		return true;
	}
}; // namespace