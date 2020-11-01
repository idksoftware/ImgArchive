
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
	
		ColumnJustification columnJustification(m_resultsList.getTableSchema().size());

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			columnJustification.readRow(row);
		}
		int filePathIdx = -1;
		int fileNameIdx = -1;
		int eventIdx = -1;
		int idx = 0;
		std::ostringstream headerStr;
		// Header
		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {

			MTSchema& columnInfo = *i;
			std::string s = columnInfo.getName();
			columnJustification.header(idx, s);
			if (columnInfo.getName().compare(DB_FILEPATH) == 0) {
				filePathIdx = idx++;
			} else if (columnInfo.getName().compare(DB_FILENAME) == 0) {
				fileNameIdx = idx++;
			}
			else {

				if (columnInfo.getName().compare(DB_EVENT) == 0) {
					eventIdx = idx;
					headerStr << std::setw(HistoryEvent::maxStringSize() + 1) << columnInfo.getName();
				}
				else {
					headerStr << std::setw(columnJustification.getSize(idx) + 1) << columnInfo.getName();
				}
				idx++;
			}
		}
		
		m_output << "\n";
		std::string currImageAddress;
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			idx = 0;
			std::string imageAddress = row->columnAt(DB_FILEPATH).toString();
			imageAddress += '/';
			imageAddress += row->columnAt(DB_FILENAME).toString();
			if (imageAddress.compare(currImageAddress) != 0) {
				currImageAddress = imageAddress;
				m_output << '\n';
				m_output << "Image: " << imageAddress << '\n';
				m_output << "----------------------------------------------------------------" << '\n';
				m_output << headerStr.str() << '\n'; 

			}
			else {
				for (auto i = row->begin(); i != row->end(); i++) {
					SharedMTColumn column = *i;
					if (filePathIdx != idx && fileNameIdx != idx) {
						if (eventIdx == idx) {
							HistoryEvent::Event evn = static_cast<HistoryEvent::Event>(column->getInt());
							m_output << std::setw(HistoryEvent::maxStringSize() + 1) << HistoryEvent::getString(evn);
						}
						else {
							m_output << std::setw(columnJustification.getSize(idx) + 1) << column->toString();
						}
					}
					idx++;
				}
				m_output << '\n';
			}

		}
		m_output << "\n\n";

		return true;
	}
	bool ImageHistoryResultsPresentation::writeHuman() {
		ImageHistoryWriteHuman imageHistoryWriteHuman(m_resultsList);
		if (!imageHistoryWriteHuman.write()) {
			return false;
		};
		std::string temp = imageHistoryWriteHuman.getOutput();
		if (write(temp) == false) {
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
		int idx = 0;
		int filePathIdx = -1;
		int fileNameIdx = -1;
		int eventIdx = -1;
		std::ostringstream headerStr;
		// Header
		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
			MTSchema& columnInfo = *i;
			if (columnInfo.getName().compare(DB_FILEPATH) == 0) {
				filePathIdx = idx++;
			}
			else if (columnInfo.getName().compare(DB_FILENAME) == 0) {
				fileNameIdx = idx++;
			}
			else if (columnInfo.getName().compare(DB_EVENT) == 0) {
				eventIdx = idx;
			}
		}

		int rowSize = tableSchema.size();
		m_output << "{\n";
		m_output << "\"images\": [\n";
		bool first = true;
		bool firstHistory = true;
		std::string currImageAddress;
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			
			std::string imageAddress = row->columnAt(DB_FILEPATH).toString();
			imageAddress += '/';
			imageAddress += row->columnAt(DB_FILENAME).toString();
			if (imageAddress.compare(currImageAddress) != 0) {
				currImageAddress = imageAddress;
				if (firstHistory == false) {
					m_output << "}]\n";
				}
				firstHistory = true;
				int idx = 0;
				if (first) {
					first = false;
				}
				else {
					m_output << "},\n";
				}
				m_output << "{\n";
				std::string column = row->columnAt(DB_FILEPATH).toString();
				m_output << writeTag(DB_FILEPATH, column.c_str(), false);
				column = row->columnAt(DB_FILENAME).toString();
				m_output << writeTag(DB_FILENAME, column.c_str(), false);
			}
			else {
				if (firstHistory) {
					firstHistory = false;
					m_output << "\"ImageHistory\": [\n";
				}
				else {
					m_output << "},\n";
				}
				m_output << "{\n";
				m_output << writeTag(DB_DATEADDED, row->columnAt(DB_DATEADDED).toString(), false);
				m_output << writeTag(DB_VERSION, row->columnAt(DB_VERSION).toString(), false);
				m_output << writeTag(DB_EVENT, row->columnAt(DB_EVENT).toString(), false);
				m_output << writeTag(DB_COMMENT, row->columnAt(DB_COMMENT).toString(), true);
			}

		}
		m_output << "}\n"; // last history item
		m_output << "] }\n"; // last image
		m_output << "] }\n"; // close main array and close main
		return true;
	};

	bool ImageHistoryResultsPresentation::writeJson() {
		ImageHistoryWriteJson systemHistoryWriteJson(m_resultsList);
		if (!systemHistoryWriteJson.write()) {
			return false;
		};
		std::string temp = systemHistoryWriteJson.getOutput();
		if (write(temp) == false) {
			return false;
		}
		return true;
	}

	

	//
	// Html
	//
	class  ImageHistoryWriteHtml : public WriteHtml {
	public:
		ImageHistoryWriteHtml(ResultsList& resultsList);
		~ImageHistoryWriteHtml() = default;
		bool write() override;
	};

	ImageHistoryWriteHtml::ImageHistoryWriteHtml(ResultsList& resultsList) : WriteHtml(resultsList) {}

	bool ImageHistoryWriteHtml::write() {
		MTTableSchema& tableSchema = m_resultsList.getTableSchema();
		int idx = 0;
		int filePathIdx = -1;
		int fileNameIdx = -1;
		int eventIdx = -1;
		std::ostringstream headerStr;
		// Header
		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
			MTSchema& columnInfo = *i;
			if (columnInfo.getName().compare(DB_FILEPATH) == 0) {
				filePathIdx = idx++;
			}
			else if (columnInfo.getName().compare(DB_FILENAME) == 0) {
				fileNameIdx = idx++;
			}
			else if (columnInfo.getName().compare(DB_EVENT) == 0) {
				eventIdx = idx;
			}
		}

		int rowSize = tableSchema.size();
		m_output << "{\n";
		m_output << "\"images\": [\n";
		bool first = true;
		bool firstHistory = true;
		std::string currImageAddress;
		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;

			std::string imageAddress = row->columnAt(DB_FILEPATH).toString();
			imageAddress += '/';
			imageAddress += row->columnAt(DB_FILENAME).toString();
			if (imageAddress.compare(currImageAddress) != 0) {
				currImageAddress = imageAddress;
				if (firstHistory == false) {
					m_output << "}]\n";
				}
				firstHistory = true;
				int idx = 0;
				if (first) {
					first = false;
				}
				else {
					m_output << "},\n";
				}
				m_output << "{\n";
				std::string column = row->columnAt(DB_FILEPATH).toString();
				//m_output << writeTag(DB_FILEPATH, column.c_str(), false);
				column = row->columnAt(DB_FILENAME).toString();
				//m_output << writeTag(DB_FILENAME, column.c_str(), false);
			}
			else {
				if (firstHistory) {
					firstHistory = false;
					m_output << "\"ImageHistory\": [\n";
				}
				else {
					m_output << "},\n";
				}
				m_output << "{\n";
				//m_output << writeTag(DB_DATEADDED, row->columnAt(DB_DATEADDED).toString(), false);
				//m_output << writeTag(DB_VERSION, row->columnAt(DB_VERSION).toString(), false);
				//m_output << writeTag(DB_EVENT, row->columnAt(DB_EVENT).toString(), false);
				//m_output << writeTag(DB_COMMENT, row->columnAt(DB_COMMENT).toString(), true);
			}

		}
		m_output << "}\n"; // last history item
		m_output << "] }\n"; // last image
		m_output << "] }\n"; // close main array and close main
		return true;
	};
	

	bool ImageHistoryResultsPresentation::writeHtml()
	{
		ImageHistoryWriteHtml imageHistoryWriteHtml(m_resultsList);
		if (!imageHistoryWriteHtml.write()) {
			return false;
		};
		std::string temp = imageHistoryWriteHtml.getOutput();
		if (write(temp) == false) {
			return false;
		}
		return true;
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
