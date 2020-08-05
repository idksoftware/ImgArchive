
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


	bool ImageHistoryResultsPresentation::writeHuman() {

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

		size_t filename = 0;
		size_t filepath = 0;
		size_t comment = 0;

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			const MTTableSchema& ts = row->getSchema(); // testing only
			MTColumn& filenameCol = row->columnAt(DB_FILENAME);
			const std::string& filenameStr = filenameCol.getString();

			MTColumn& filepathCol = row->columnAt(DB_FILEPATH);
			const std::string& filepathStr = filepathCol.getString();

			MTColumn& commentCol = row->columnAt(DB_COMMENT);
			const std::string& commentStr = commentCol.getString();

			if (filenameStr.length() > filename) {
				filename = filenameStr.length();
			}

			if (filepathStr.length() > filepath) {
				filepath = filepathStr.length();
			}

			if (commentStr.length() > comment) {
				comment = commentStr.length();
			}
		}

		for (std::vector<MTSchema>::iterator i = m_resultsList.getTableSchema().begin(); i != m_resultsList.getTableSchema().end(); i++) {
			MTSchema& columnInfo = *i;
			//printf("%s ", columnInfo.getName().c_str());
			std::cout << ' ';
			//	first = false;
			//}  setw
			if (columnInfo.getName().compare(DB_DATEADDED) == 0) {
				std::cout << std::setw(19) << columnInfo.getName().c_str();
			}
			else if (columnInfo.getName().compare(DB_FILENAME) == 0) {
				std::cout << std::setw(filename) << columnInfo.getName().c_str();
			}
			else if (columnInfo.getName().compare(DB_FILEPATH) == 0) {
				std::cout << std::setw(filepath) << columnInfo.getName().c_str();
			}
			else if (columnInfo.getName().compare(DB_COMMENT) == 0) {
				std::cout << std::setw(comment) << columnInfo.getName().c_str();
			}

			else {
				std::cout << columnInfo.getName().c_str();
			}
		}
		std::cout << '\n';
		//bool first = true;

		for (auto rowIt = m_resultsList.begin(); rowIt != m_resultsList.end(); rowIt++) {
			SharedMTRow row = *rowIt;
			auto schemaIdx = m_resultsList.getTableSchema().begin();
			for (auto i = row->begin(); i != row->end(); i++) {
				SharedMTColumn column = *i;
				MTSchema& columnInfo = *schemaIdx;
				//std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
				//std::cout << mtSchema->getName();
				//if (mtSchema == nullptr) {
				//	continue;
				//}
				//if (!first) {
				std::cout << ' ';
				//	first = false;
				//}  setw

				if (columnInfo.getName().compare(DB_FILENAME) == 0) {
					std::cout << std::setw(filename) << column->toString();
				}
				else if (columnInfo.getName().compare(DB_FILEPATH) == 0) {
					std::cout << std::setw(filepath) << column->toString();
				}
				else if (columnInfo.getName().compare(DB_COMMENT) == 0) {
					std::cout << std::setw(comment) << column->toString();
				}
				else if (columnInfo.getName().compare(DB_EVENT) == 0) {
					HistoryEvent::Event ev = (HistoryEvent::Event)column->getInt();
					HistoryEvent historyEvent(ev);
					std::cout << std::setw(10) << historyEvent.getString();
				}
				else {
					std::cout << column->toString();
				}
				schemaIdx++;
			}
			std::cout << '\n';
		}

		return true;
	}


	bool ImageHistoryResultsPresentation::writeJson() {
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