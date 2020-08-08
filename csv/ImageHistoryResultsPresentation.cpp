
#include <iostream>
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"
#include <iomanip>
#include "DBDefines.h"
#include "HistoryEvent.h"
#include "ImageHistoryResultsPresentation.h"
#include "ResultsList.h"
#include "ResultsPresentation.h"

namespace simplearchive {

	//
	// WriteHuman
	//
	class  ImageHistoryWriteHuman : public WriteHuman {

	public:
		ImageHistoryWriteHuman(ResultsList& resultsList);
		~ImageHistoryWriteHuman() = default;

		bool write() override;
	};

	ImageHistoryWriteHuman::ImageHistoryWriteHuman(ResultsList& resultsList) : WriteHuman(resultsList) {}

	bool ImageHistoryWriteHuman::write() {
	

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

		return true;
	}
	bool ImageHistoryResultsPresentation::writeHuman() {
		ImageHistoryWriteHuman imageHistoryWriteHuman(m_resultsList);
		if (!imageHistoryWriteHuman.write()) {
			return false;
		};
		if (write(imageHistoryWriteHuman.getOutput()) == false) {
			return false;
		}
		return true;
	}

	//
	// WriteJSON
	//
	class  ImageHistoryWriteJson : public WriteJSON {
	public:
		ImageHistoryWriteJson(ResultsList& resultsList);
		~ImageHistoryWriteJson() = default;
		bool write() override;
	};

	ImageHistoryWriteJson::ImageHistoryWriteJson(ResultsList& resultsList) : WriteJSON(resultsList) {}

	bool ImageHistoryWriteJson::write() {
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

	bool ImageHistoryResultsPresentation::writeJson() {
		ImageHistoryWriteJson systemHistoryWriteJson(m_resultsList);
		if (!systemHistoryWriteJson.write()) {
			return false;
		};
		if (write(systemHistoryWriteJson.getOutput()) == false) {
			return false;
		}
		return true;
	}

	

	bool ImageHistoryResultsPresentation::writeHtml()
	{
		return false;
	}

	bool ImageHistoryResultsPresentation::writeCSV() {
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			std::cout << *rowIt << '\n';
		}
		return true;
	}

	class ImageHistoryWriteXML : WriteXML {
	public:
		ImageHistoryWriteXML(ResultsList& resultsList);
		~ImageHistoryWriteXML() = default;

		bool write() override;
	};

	ImageHistoryWriteXML::ImageHistoryWriteXML(ResultsList& resultsList) : WriteXML(resultsList) {}

	bool ImageHistoryWriteXML::write() {

		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";


		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			std::cout << "\t<Event>\n";
			for (size_t i = 0; i != row->size(); i++) {
				MTTableSchema tableSchema = m_resultsList.getTableSchema();
				std::cout << writeTag(tableSchema.getColumnName(i).c_str(), row->columnAt(i).toString(), 2);
			}
			std::cout << "\t</Event>\n";
		}

		std::cout << "</Catalog>\n";
		return true;
	}

	bool ImageHistoryResultsPresentation::writeXML() {
		ImageHistoryWriteXML imageHistoryWriteXML(m_resultsList);
		if (!imageHistoryWriteXML.write()) {
			return false;
		}
		return true;
	}
}; // namespace